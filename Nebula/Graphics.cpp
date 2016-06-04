//
//  Graphics.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/raspberryPi.h>
#include <raspberrypi/Graphics.h>
#include <raspberrypi/Memory.h>
#include <raspberrypi/Mailbox.h>
#include <raspberrypi/DMA.h>
#include <raspberrypi/UART.h>
#include "fonts.h"

using namespace raspberryPi;
using namespace Memory;

volatile
struct frame_buffer_info_t {
    uint32_t screenWidth;
    uint32_t screenHeight;
    uint32_t virtualWidth;
    uint32_t virtualHeight;
    uint32_t pitch;
    uint32_t depth;
    uint32_t xOffset;
    uint32_t yOffset;
    uint32_t bufferPtr;
    uint32_t bufferSize;
} frame_buffer_info __attribute__ ((aligned (16)));

volatile
struct screen_info_t {
    uint32_t screenEnabled;
    uint32_t cursorX;
    uint32_t cursorY;
    uint32_t foreground;
    uint32_t background;
    uint32_t maxX;
    uint32_t maxY;
    uint32_t charSizeX;
    uint32_t charSizeY;
    Graphics::pixel_order_t pixelOrder;
    uint8_t doubleFrameBuffer;
    uint8_t currentFrameBuffer;
} __attribute__ ((aligned (16))) screen_info;

static uint32_t colorMap[][8] = {
    {
        0,0,0,0,0,0,0,0
    },
    {
        0b1111100000000000,
        0b0000011111100000,
        0b0000000000011111,
        0b1111111111100000,
        0b1111100000011111,
        0b0000011111111111,
        0b1111111111111111,
        0b0000000000000000
    },
    {
        0xFF0000,
        0x00FF00,
        0x0000FF,
        0xFFFF00,
        0xFF00FF,
        0x00FFFF,
        0xFFFFFF,
        0x000000
    },
    {
        0xFF000000,
        0x00FF0000,
        0xAAF5FF00,
        0xFFFF0000,
        0xFF00FF00,
        0x00FFFF00,
        0xFFFFFF00,
        0x00000000
    }
};

bool Graphics::init(uint32_t width, uint32_t height, uint32_t depth, bool doubleFrameBuffer) {
    if (width > 4096) return false;
    if (height > 4096) return false;
    if (depth > 32) return false;

    frame_buffer_info.screenWidth   = width;
    frame_buffer_info.screenHeight  = height;
    frame_buffer_info.virtualWidth  = width;
    frame_buffer_info.virtualHeight = height;
    
    if (doubleFrameBuffer) {
        frame_buffer_info.virtualHeight += height;
        screen_info.doubleFrameBuffer    = 1;
        screen_info.currentFrameBuffer   = 2;
    } else {
        screen_info.doubleFrameBuffer    = 0;
        screen_info.currentFrameBuffer   = 1;
    }
    
    frame_buffer_info.depth = depth;
    frame_buffer_info.xOffset = 0;
    frame_buffer_info.yOffset = 0;
    frame_buffer_info.pitch = 0;
    frame_buffer_info.bufferPtr = 0;
    frame_buffer_info.bufferSize = 0;
    
    uint32_t request = ((uint32_t)&frame_buffer_info);
    
    raspberryPi::UART::write("frame_buffer_info Bus address: ");
    raspberryPi::UART::writeHexStringLine(request);
    
    Mailbox::write(1, request);
    
    uint32_t result = 0xFF;
    do {
        result = Mailbox::read(1);
    } while (result != 0);
    
    raspberryPi::UART::write("bufferPtr Virtual address: ");
    raspberryPi::UART::writeHexStringLine(frame_buffer_info.bufferPtr);
    
    raspberryPi::UART::write("bufferPtr Physical address: ");
    raspberryPi::UART::writeHexStringLine(addressToPhysical(frame_buffer_info.bufferPtr));
    
    raspberryPi::UART::write("pitch: ");
    raspberryPi::UART::writeHexStringLine(frame_buffer_info.pitch);
    if (frame_buffer_info.bufferPtr == 0) return false;
    if (frame_buffer_info.pitch == 0) return false;
    
    frame_buffer_info.bufferPtr = addressToPhysical(frame_buffer_info.bufferPtr);
    
    screen_info.charSizeX = 6;
    screen_info.charSizeY = 10;
    screen_info.screenEnabled = YES;
    screen_info.cursorX = 0;
    screen_info.cursorY = 0;
    screen_info.background = 0x00000000;
    screen_info.maxX = div(width, screen_info.charSizeX);
    screen_info.maxY = div(height, screen_info.charSizeY);
    screen_info.pixelOrder = detectPixelOrder();
    
    if (frame_buffer_info.depth == 16) {
        screen_info.foreground = 0xFFFF;
    } else if (frame_buffer_info.depth == 24) {
        screen_info.foreground = 0xFFFFFFFF;
    } else if (frame_buffer_info.depth == 32) {
        screen_info.foreground = 0xFFFFFF00;
        uint32_t param[1] = { 0 };
        raspberryPi::Mailbox::property(raspberryPi::Mailbox::ALPHA_MODE_SET, param);
    }
    
    return true;
}

void Graphics::lockFoucs() {
    // 是否启用了双缓冲
    if (screen_info.doubleFrameBuffer & 0x1) {
        // 是否已经lockFoucs
        if ((screen_info.doubleFrameBuffer & 0x10) == 0) {
            // 还没有lockFoucs, 设定标志位
            screen_info.doubleFrameBuffer |= 0x10;
            
            // 切换当前缓冲序号
            if (screen_info.currentFrameBuffer == 2) {
                screen_info.currentFrameBuffer = 1;
            } else {
                screen_info.currentFrameBuffer = 2;
            }
        }
    }
}

void Graphics::unlockFoucs() {
    // 是否启用了双缓冲
    if (screen_info.doubleFrameBuffer & 0x1) {
        // 是否已经lockFoucs
        if ((screen_info.doubleFrameBuffer & 0x10) != 0) {
            // 有lockFoucs, 取消标志位
            screen_info.doubleFrameBuffer = 0x1;
            
            // 切换当前缓冲序号
            if (screen_info.currentFrameBuffer == 2) {
                screen_info.currentFrameBuffer = 1;
            } else {
                screen_info.currentFrameBuffer = 2;
            }
        }
    }
}

static struct CB loadFrameBuffer_control_blocks __attribute__ ((aligned (256)));

void Graphics::loadFrameBuffer(uint32_t * address) {
    uint32_t length =  (frame_buffer_info.screenHeight * frame_buffer_info.pitch);
    
    uint32_t bufferOffset = 0;
    if (screen_info.doubleFrameBuffer & 0x1) {
        if (screen_info.currentFrameBuffer == 2) {
            bufferOffset = length;
        }
    }
    
    // No interrupt
    // YES 2D mode
    // Don't wait
    // Dest inc 32
    // Dest width 128
    // No dest dreq
    // Write
    // Src inc 32
    // Src width 128
    // No src dreq
    // Read
    // burst length 8 words
    // No premap
    // No waits
    // Dont do wide writes as a 2 beat burst
    
    // 00000 0 | 0000 0000 | 1000 0 0 1 1 | 0 0 1 1 0 0 1 0
    loadFrameBuffer_control_blocks.info = 0x8332;
    loadFrameBuffer_control_blocks.src = addressToPhysical((uint32_t)address);
    loadFrameBuffer_control_blocks.dest = addressToPhysical((frame_buffer_info.bufferPtr + bufferOffset));
    loadFrameBuffer_control_blocks.length = ((0xE1 << 16) | 0x1000);
    loadFrameBuffer_control_blocks.mode2D = 0;
    loadFrameBuffer_control_blocks.next = 0;
    
    DMA::sendDMARequest(&loadFrameBuffer_control_blocks, 0);
    
    //    uint32_t pos = 0;
    //    for (uint32_t i = 0; i < frame_buffer_info.screenHeight * frame_buffer_info.pitch + frame_buffer_info.screenWidth * (frame_buffer_info.depth / 8); i+=4) {
    //        *(uint32_t *)(frame_buffer_info.bufferPtr + bufferOffset + i) = __builtin_bswap32(address[pos]);
    //        pos++;
    //    }
}

void Graphics::drawPixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t bufferOffset = y * frame_buffer_info.pitch + x * (frame_buffer_info.depth / 8);
    
    if (screen_info.doubleFrameBuffer & 0x1) {
        if (screen_info.currentFrameBuffer == 2) {
            bufferOffset += frame_buffer_info.screenHeight * frame_buffer_info.pitch + frame_buffer_info.screenWidth * (frame_buffer_info.depth / 8);
        }
    }
    
    if (frame_buffer_info.depth == 16) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = (color & 0x001F);
        
        uint16_t colorInOrder;
        if (screen_info.pixelOrder == Graphics::BGR) {
            colorInOrder = ((b << 11) | (g << 5) | r);
        } else {
            colorInOrder = ((r << 11) | (g << 5) | b);
        }
        
        *(uint16_t*)(frame_buffer_info.bufferPtr + bufferOffset) = colorInOrder;
    } else if (frame_buffer_info.depth == 24) {
        uint8_t r = (color & 0xFF0000) >> 16;
        uint8_t g = (color & 0x00FF00) >> 8;
        uint8_t b = (color & 0x0000FF);
        
        if (screen_info.pixelOrder == Graphics::BGR) {
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 0) = b;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 1) = g;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 2) = r;
        } else {
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 0) = r;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 1) = g;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 2) = b;
        }
    } else if (frame_buffer_info.depth == 32) {
        uint8_t r = (color & 0xFF000000) >> 24;
        uint8_t g = (color & 0x00FF0000) >> 16;
        uint8_t b = (color & 0x0000FF00) >> 8;
        uint8_t a = (color & 0x000000FF);
        
        if (screen_info.pixelOrder == Graphics::BGR) {
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 0) = b;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 1) = g;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 2) = r;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 3) = a;
        } else {
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 0) = r;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 1) = g;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 2) = b;
            *(uint8_t*)(frame_buffer_info.bufferPtr + bufferOffset + 3) = a;
        }
    }
}

void Graphics::sync() {
    if (screen_info.doubleFrameBuffer & 0x1) {
        if ((screen_info.doubleFrameBuffer & 0x10) != 0) return;
        
        uint32_t params[2] = {0, 0};
        
        if (screen_info.currentFrameBuffer == 2) {
            params[1] = frame_buffer_info.screenHeight;
            screen_info.currentFrameBuffer = 1;
        } else {
            params[1] = 0;
            screen_info.currentFrameBuffer = 2;
        }
        
        Mailbox::property(Mailbox::VIRTUAL_OFFSET_SET, params);
    }
}

void Graphics::clear(uint32_t color) {
    uint32_t bufferOffset = 0;
    if (screen_info.doubleFrameBuffer & 0x1) {
        if (screen_info.currentFrameBuffer == 2) {
            bufferOffset += (frame_buffer_info.screenHeight * frame_buffer_info.pitch);
        }
    }
    
    if (frame_buffer_info.depth == 16) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = (color & 0x001F);
        
        uint16_t colorInOrder;
        if (screen_info.pixelOrder == Graphics::BGR) {
            colorInOrder = ((b << 11) | (g << 5) | r);
        } else {
            colorInOrder = ((r << 11) | (g << 5) | b);
        }
        
        for (uint32_t offset = 0; offset < (frame_buffer_info.screenHeight * frame_buffer_info.pitch + frame_buffer_info.screenWidth * 2); offset++) {
            *(uint16_t*)(frame_buffer_info.bufferPtr + offset + bufferOffset) = colorInOrder;
        }
    } else if (frame_buffer_info.depth == 24) {
        __attribute__ ((aligned (8)))
        struct clear_t {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } clear;
        clear.r = (color & 0xFF0000) >> 16;
        clear.g = (color & 0x00FF00) >> 8;
        clear.b = (color & 0x0000FF);
        if (screen_info.pixelOrder == Graphics::BGR)
            clear.r ^= clear.b ^= clear.r ^= clear.b;
        for (uint32_t offset = 0; offset < (frame_buffer_info.screenHeight * frame_buffer_info.pitch + frame_buffer_info.screenWidth * 3); offset+=3) {
            *(clear_t*)(frame_buffer_info.bufferPtr + offset + bufferOffset) = clear;
        }
    } else if (frame_buffer_info.depth == 32) {
        uint8_t r = (color & 0xFF000000) >> 24;
        uint8_t g = (color & 0x00FF0000) >> 16;
        uint8_t b = (color & 0x0000FF00) >> 8;
        uint8_t a = (color & 0x000000FF);
        
        uint32_t colorInOrder;
        if (screen_info.pixelOrder == Graphics::BGR) {
            colorInOrder = ((b << 24) | (g << 16) | (r << 8) | a);
        } else {
            colorInOrder = ((r << 24) | (g << 16) | (b << 8) | a);
        }
        
        for (uint32_t offset = 0; offset < (frame_buffer_info.screenHeight * frame_buffer_info.pitch + frame_buffer_info.screenWidth * 4); offset+=4) {
            *(uint32_t*)(frame_buffer_info.bufferPtr + offset + bufferOffset) = colorInOrder;
        }
    }
}

void Graphics::resolution(uint32_t *width, uint32_t *height, uint32_t of) {
    if (of == CURRENT_PHY) {
        if (width)  *width  = frame_buffer_info.screenWidth;
        if (height) *height = frame_buffer_info.screenHeight;
    } else if (of == CURRENT_VIR) {
        if (width)  *width  = frame_buffer_info.virtualWidth;
        if (height) *height = frame_buffer_info.virtualHeight;
    } else if (of == PHYSICAL) {
        volatile uint32_t * result = Mailbox::property(Mailbox::PHYSICAL_SIZE_GET);
        if (result[0] != 0) {
            if (width)  *width  = result[1];
            if (height) *height = result[2];
        }
    } else if (of == VIRTUAL) {
        volatile uint32_t * result = Mailbox::property(Mailbox::VIRTUAL_SIZE_GET);
        if (result[0] != 0) {
            if (width)  *width  = result[1];
            if (height) *height = result[2];
        }
    }
}

Graphics::pixel_order_t Graphics::pixelOrder() {
    return screen_info.pixelOrder;
}

Graphics::pixel_order_t Graphics::detectPixelOrder() {
    volatile uint32_t * result = Mailbox::property(Mailbox::PIXEL_ORDER_GET);
    if (result[0] != 0) {
        if (result[1] == 0x1) {
            return RGB;
        } else if (result[1] == 0x0) {
            return BGR;
        }
    }
    return ERROR_WHILE_GET;
}

void Graphics::setPixelOrder(pixel_order_t order) {
    screen_info.pixelOrder = order;
}

/* A small stack to allow temporary colour changes in text */
static uint64_t colour_stack[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int colour_sp = 8;

void Graphics::Console::lineFeed() {
    uint32_t source;
    register uint32_t rowBytes = screen_info.charSizeY * frame_buffer_info.pitch;
    
    screen_info.cursorX = 0;
    if (screen_info.cursorY < (screen_info.maxY - 1)) {
        screen_info.cursorY++;
        return;
    }
    
    uint32_t bufferOffset = 0;
    if (screen_info.doubleFrameBuffer & 0x1) {
        if (screen_info.currentFrameBuffer == 2) {
            bufferOffset += (frame_buffer_info.screenHeight * frame_buffer_info.pitch);
        }
    }
    
    source = frame_buffer_info.bufferPtr + rowBytes + bufferOffset;
    memmove((void *)frame_buffer_info.bufferPtr, (void *)source, (screen_info.maxY - 1) * rowBytes);
    memclr((void *)(frame_buffer_info.bufferPtr + (screen_info.maxY - 1) * rowBytes), rowBytes);
}

void Graphics::Console::logc(char c) {
    uint32_t bufferOffset = 0;
    
    if (screen_info.doubleFrameBuffer) {
        if (screen_info.currentFrameBuffer == 2) {
            bufferOffset = (frame_buffer_info.screenHeight * frame_buffer_info.pitch);
        }
    }
    
    uint32_t row, addr;
    int col;
    
    if(c < 32) c = 0;
    else if(c > 127) c = 0;
    else c -= 32;
    
    if (frame_buffer_info.depth == 16) {
        // 前景色, 背景色的三个分量
        uint8_t fR, fG, fB;
        uint8_t bR, bG, bB;
        
        // 计算前景色和背景色
        uint16_t fColor, bColor;
        fR = (screen_info.foreground & 0xF800) >> 11;
        fG = (screen_info.foreground & 0x07E0) >> 5;
        fB = (screen_info.foreground & 0x001F);
        
        bR = (screen_info.background & 0xF800) >> 11;
        bG = (screen_info.background & 0x07E0) >> 5;
        bB = (screen_info.background & 0x001F);
        
        // 调整颜色表示
        if (screen_info.pixelOrder == Graphics::BGR) {
            fColor = ((fB << 11) | (fG << 5) | fR);
            bColor = ((fB << 11) | (fG << 5) | fR);
        } else {
            fColor = ((fR << 11) | (fG << 5) | fB);
            bColor = ((fR << 11) | (fG << 5) | fB);
        }
        
        for(row = 0; row < screen_info.charSizeY; row++) {
            addr = (row+screen_info.cursorY * screen_info.charSizeY) * frame_buffer_info.pitch + screen_info.cursorX * screen_info.charSizeX * 2;
            for(col = (screen_info.charSizeX - 2); col >= 0; col--) {
                if (row < (screen_info.charSizeY - 1) && (fonts[c][row] & (1 << col))){
                    *(uint16_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = fColor;
                } else {
                    *(uint16_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
                }
                addr += 2;
            }
            *(uint16_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
        }
    } else if (frame_buffer_info.depth == 24) {
        // 前景色, 背景色的三个分量
        __attribute__ ((aligned (8)))
        struct _color_t {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } fColor, bColor;
        
        // 计算前景色和背景色
        fColor.r = (screen_info.foreground & 0xFF0000) >> 16;
        fColor.g = (screen_info.foreground & 0x00FF00) >> 8;
        fColor.b = (screen_info.foreground & 0x0000FF);
        
        bColor.r = (screen_info.background & 0xFF0000) >> 16;
        bColor.g = (screen_info.background & 0x00FF00) >> 8;
        bColor.b = (screen_info.background & 0x0000FF);
        
        // 调整颜色表示
        if (screen_info.pixelOrder == Graphics::BGR){
            fColor.r ^= fColor.b ^= fColor.r ^= fColor.b;
            bColor.r ^= bColor.b ^= bColor.r ^= bColor.b;
        }
        
        for(row = 0; row < screen_info.charSizeY; row++) {
            addr = (row + screen_info.cursorY * screen_info.charSizeY) * frame_buffer_info.pitch + screen_info.cursorX * screen_info.charSizeX * 3;
            for(col = (screen_info.charSizeX - 2); col >= 0; col--) {
                if (row < (screen_info.charSizeY - 1) && (fonts[c][row] & (1 << col))) {
                    *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = fColor;
                } else {
                    *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
                }
                addr += 3;
            }
            *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
        }
    } else if (frame_buffer_info.depth == 32) {
        // 前景色, 背景色的四个分量
        uint8_t fA, fR, fG, fB;
        uint8_t bA, bR, bG, bB;
        
        // 计算前景色和背景色
        fR = (screen_info.foreground & 0xFF000000) >> 24;
        fG = (screen_info.foreground & 0x00FF0000) >> 16;
        fB = (screen_info.foreground & 0x0000FF00) >> 8;
        fA = (screen_info.foreground & 0x000000FF);
        
        bR = (screen_info.background & 0xFF000000) >> 24;
        bG = (screen_info.background & 0x00FF0000) >> 16;
        bB = (screen_info.background & 0x0000FF00) >> 8;
        bA = (screen_info.background & 0x000000FF);
        
        __attribute__ ((aligned (32)))
        struct _color_t {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } fColor = {
            fR,
            fG,
            fB,
            fA
        }, bColor = {
            bR,
            bG,
            bB,
            bA
        };
        
        // 调整颜色表示
        if (screen_info.pixelOrder == Graphics::BGR) {
            fColor.r ^= fColor.b ^= fColor.r ^= fColor.b;
            bColor.r ^= bColor.b ^= bColor.r ^= bColor.b;
        }
        
        for(row = 0; row < screen_info.charSizeY; row++) {
            addr = (row+screen_info.cursorY * screen_info.charSizeY) * frame_buffer_info.pitch + screen_info.cursorX * screen_info.charSizeX * 4;
            for(col = (screen_info.charSizeX - 2); col >= 0; col--) {
                if (row < (screen_info.charSizeY - 1) && (fonts[c][row] & (1 << col))){
                    *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = fColor;
                } else {
                    *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
                }
                addr += 4;
            }
            *(_color_t*)(frame_buffer_info.bufferPtr + addr + bufferOffset) = bColor;
        }
    }
    
    if(++screen_info.cursorX >= screen_info.maxX) lineFeed();
}

void Graphics::Console::log(const char *log) {
    UART::writeLine(log);
    unsigned char ch;
    
    /* Double parentheses to silence compiler warnings about
     * assignments as boolean values
     */
    while((ch = (unsigned char)*log)) {
        log++;
        
        /* Deal with control codes */
        uint8_t colorMapEntry = (frame_buffer_info.depth / 8) - 1;
        switch(ch) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: screen_info.foreground = colorMap[colorMapEntry][ch - 1]; continue;
                /* Half brightness */
            case 9: screen_info.foreground = (screen_info.foreground >> 1) & 0b0111101111101111; continue;
            case 10: lineFeed(); continue;
            case 11: /* Colour stack push */
                if(colour_sp)
                    colour_sp--;
                colour_stack[colour_sp] = screen_info.background;
                colour_stack[colour_sp] <<= 32;
                colour_stack[colour_sp] |= screen_info.foreground;
                continue;
            case 12: /* Colour stack pop */
                screen_info.foreground = colour_stack[colour_sp] & 0xFFFFFFFF;
                screen_info.background = colour_stack[colour_sp] >> 32;
                if(colour_sp < 8)
                    colour_sp++;
                continue;
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24: screen_info.background = colorMap[colorMapEntry][ch -17]; continue;
                /* Half brightness */
            case 25: screen_info.background = (screen_info.background >> 1) & 0b0111101111101111; continue;
        }
        logc(ch);
    }
}

void Graphics::Console::logHexString(uint32_t data) {
    unsigned int rb;
    unsigned int rc;
    rb = 32;
    while(1) {
        rb -= 4;
        rc = (data >> rb) & 0xF;
        if(rc > 9) rc += 0x37;
        else rc += 0x30;
        logc(rc);
        if(rb == 0) break;
    }
    logc(0x20);
}

void Graphics::Console::logHexStringLine(uint32_t data) {
    logHexString(data);
    lineFeed();
}

void cursorBackspace() {
    if (screen_info.cursorX == 0) {
        if (screen_info.cursorY == 0) return;
        screen_info.cursorX = screen_info.maxX - 1;
        screen_info.cursorY--;
    } else {
        screen_info.cursorX -= 1;
    }
}

void Graphics::Console::backspace() {
    cursorBackspace();
    log(COLOUR_PUSH FG_WHITE BG_BLACK " " COLOUR_POP);
    cursorBackspace();
}

void Graphics::Console::clear() {
    Graphics::clear();
    screen_info.cursorX = 0;
    screen_info.cursorY = 0;
}

void Graphics::Console::nls(raspberryPi::Graphics::Console::nlt_t level, const char * nls) {
    if (level == OK) {
        log("[" COLOUR_PUSH FG_GREEN "OK" COLOUR_POP "] ");
        log(nls);
    } else if (level == INFO) {
        log("[" COLOUR_PUSH FG_BLUE "INFO" COLOUR_POP "] ");
        log(nls);
    } else if (level == WARNING) {
        log("[" COLOUR_PUSH FG_YELLOW "WARNING" COLOUR_POP "] ");
        log(nls);
    } else if (level == FAILED) {
        log("[" COLOUR_PUSH FG_RED "FAILED" COLOUR_POP "] ");
        log(nls);
    }
}
