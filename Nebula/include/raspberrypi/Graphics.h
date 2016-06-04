//
//  Graphics.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <sys/types.h>

#define FG_RED      "\001"
#define FG_GREEN    "\002"
#define FG_BLUE     "\003"
#define FG_YELLOW   "\004"
#define FG_MAGENTA  "\005"
#define FG_CYAN     "\006"
#define FG_WHITE    "\007"
#define FG_BLACK    "\010"
#define FG_HALF     "\011"

#define COLOUR_PUSH "\013"
#define COLOUR_POP  "\014"

#define BG_RED      "\021"
#define BG_GREEN    "\022"
#define BG_BLUE     "\023"
#define BG_YELLOW   "\024"
#define BG_MAGENTA  "\025"
#define BG_CYAN     "\026"
#define BG_WHITE    "\027"
#define BG_BLACK    "\030"
#define BG_HALF     "\031"

namespace raspberryPi {
    /**
     *  @brief  所有传入颜色默认为RGB(A)
     */
    namespace Graphics {
        /**
         *  @brief  初始化图形
         *
         *  @param  width  水平分辨率
         *  @param  height 垂直分辨率
         *  @param  depth  位深度
         *  @param  doubleFrameBuffer 是否启用双缓冲
         *
         *  @discussion 
         *              如果启用了双缓冲, 那么在绘图时的调用顺序如下
         *                 (1) init         完成init后, 显示的是第一个framebuffer
         *                 (2) drawPixel    但是此时是画在第二个framebuffer上的
         *                 (3) sync         此时才将显示第二个framebuffer
         *                 (4) drawPixel    此时是画在第一个framebuffer上的
         *                     ...
         *              如此循环即可
         */
        bool init(uint32_t width, uint32_t height, uint32_t depth, bool doubleFrameBuffer);
        
        /**
         *  @brief  在开启了双缓冲模式时, 为了方便将显示器用作Console, 提供了lockFoucs
         *          缓冲区将锁定为当前正显示在屏幕上的framebuffer, 模拟单缓冲模式
         *          在锁定之后调用drawPixel等等均是发生在当前正显示于屏幕上的framebuffer上
         *          如果需要返回双缓冲模式, 必须调用unlockFoucs, 跳过unlockFoucs直接调用sync是无效的
         *
         *  @discussion
         *              如果启用了双缓冲, 那么在Console时的调用顺序如下
         *                 (1) init         完成init后, 显示的是第一个framebuffer
         *                 (2) log          此时是log在第二个framebuffer上的
         *                 (3) sync         此时才将显示第二个framebuffer
         *                 (4) lockFoucs    为了方便Console, 锁定为当前正在显示的framebuffer
         *                 (5) log          此时是log在当前正在显示的framebuffer上的, 显示是同步的
         *                 (6) unlockFoucs  调用unlockFoucs返回双缓冲模式
         */
        void lockFoucs();
        
        /**
         *  @brief  解除framebuffer锁定, 仅在开启了双缓冲模式时有效
         *          调用unlockFoucs仅是解除对framebuffer的锁定而不会自动调用sync
         */
        void unlockFoucs();
        
        /**
         *  @brief  直接从内存中加载到framebuffer
         *
         *  @param  address 给定的内存起始地址, 长度将根据当前实际屏幕大小及位深度决定
         *
         *  @discussion
         *              如果启用了双缓冲, 但未锁定framebuffer, 那么将加载到第二个framebuffer上
         *              (如果启用了双缓冲, 且锁定framebuffer) || (单缓冲模式), 那么将加载到正在显示的framebuffer上
         */
        void loadFrameBuffer(uint32_t * address);
        
        /**
         *  @brief  操作像素点
         *
         *  @param x     x方向/水平方向
         *  @param y     y方向/垂直方向
         *  @param color 这个像素点的颜色
         *
         *  @discussion  左上角为(0, 0)
         *               如果启用了双缓冲, drawPixel是发生在第二个framebuffer上的
         */
        void drawPixel(uint32_t x, uint32_t y, uint32_t color);
        
        /**
         *  @brief  将第二个framebuffer显示到屏幕上, 仅在启用了双缓冲时有效
         *
         *  @discussion 原来的第一个framebuffer数据保留
         */
        void sync();
        
        /**
         *  @brief  清屏
         *
         *  @param  color 清屏后整个屏幕的颜色, 默认为黑色
         *
         *  @discussion 如果启用了双缓冲, 清屏是发生在第二个framebuffer上的
         */
        void clear(uint32_t color = 0);
        
        enum {
            CURRENT_PHY  = 0x00000001,
            CURRENT_VIR  = 0x00000010,
            PHYSICAL     = 0x00000100,
            VIRTUAL      = 0x00001000
        };
        void resolution(uint32_t * width, uint32_t * height, uint32_t of);
        enum pixel_order_t {
            BGR = 0x0,
            RGB = 0x1,
            ERROR_WHILE_GET = 0x2
        };
        pixel_order_t pixelOrder();
        pixel_order_t detectPixelOrder();
        void setPixelOrder(pixel_order_t order);
    }
}

namespace raspberryPi {
    namespace Graphics {
        namespace Console {
            void logc(char c);
            void log(const char * log);
            void logHexString(uint32_t data);
            void logHexStringLine(uint32_t data);
            void lineFeed();
            void backspace();
            void clear();
            enum nlt_t {
                OK      = 0x0,
                INFO    = 0x1,
                WARNING = 0x2,
                FAILED  = 0x3
            };
            void nls(nlt_t level, const char * log);
        }
    }
}

#endif /* __GRAPHICS_H */
