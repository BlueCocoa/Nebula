//
//  main.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/8.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/pi.h>
#include <sys/types.h>
#include "string.h"
#include "imageData.h"

//void print_0xBBC() {
//    raspberryPi::Graphics::Console::log("                                                                                                         \n");
//    raspberryPi::Graphics::Console::log("                                             ...,,,11,.             ...,,,11,.                 .246641   \n");
//    raspberryPi::Graphics::Console::log("           ,.                            7@@############72      6@@############83            ,7##@9@##92 \n");
//    raspberryPi::Graphics::Console::log("         18###71                         1327#@,,....,15##4     1326#@,,....,15##5          5##5,   .6##3\n");
//    raspberryPi::Graphics::Console::log("      27##@874@#6                           5#9        ,##4        4#9        ,##4         7#9,       ,1.\n");
//    raspberryPi::Graphics::Console::log("    ,8#@4,     8#8                          6#8      17#@3         6#9      17#@3         7#9.           \n");
//    raspberryPi::Graphics::Console::log("   2##5         9#5                         6#8  .26@#93           6#9   26@#93          5##,            \n");
//    raspberryPi::Graphics::Console::log("  ,##3          1##.     24,                6#948@@83.             6#@47@@84.           ,##4             \n");
//    raspberryPi::Graphics::Console::log("  8#6            9#3     5##5      16@3     6#####8544321.         6#####8544321.       6##              \n");
//    raspberryPi::Graphics::Console::log("  ##1            8#4      2@#@1  39##6.     6#95567889@@##94       6#@5567889@@##94.    @#7              \n");
//    raspberryPi::Graphics::Console::log("  ##,            9#3        2##79#93.       6#7         .2@#8      6#7         .2@#8   .##5              \n");
//    raspberryPi::Graphics::Console::log("  9#4           2##.        2@###2          6#7           9#8      6#8           8#9    @#7              \n");
//    raspberryPi::Graphics::Console::log("  3#@,         1##3      ,5##937#8.         7#7       ,37##7.      6#8       ,37##7.    6##,          ,2.\n");
//    raspberryPi::Graphics::Console::log("   6##3      ,5##4     .6##81   7#9.        6#7 ,2469###94,        6#8 ,1469###95,      .8#@3.   .,36@##6\n");
//    raspberryPi::Graphics::Console::log("    39##9778@##6,      8#81      7#8     .35@######9741.        .35@######9741.           5@#########841 \n");
//    raspberryPi::Graphics::Console::log("      ,3566541         ,,         11     1@@86421.              19@86431.                   1345431.     \n");
//}

//void OSInfo() {
//    raspberryPi::Graphics::Console::log(OSName" is booting...\n");
//    raspberryPi::Graphics::Console::log("Current version " Version", build " OSBuild", " "compile time: " __DATE__ " " __TIME__ "\n");
//}

//extern "C"
//void kernel_start_slave() {
//    // 读取MPIDR
//    uint32_t nMPIDR;
//    asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (nMPIDR));
//    
//    unsigned nCore = nMPIDR & 3;
//    asm volatile("dmb" ::: "memory");
//    PUT32(0xE000 + (nCore * 4), 1);
//    
//    // 清除这个CPU的L1数据缓存
//    raspberryPi::CPU::invalidateL1DataCache();
//    
//    // 启用这个CPU的FPU
//    raspberryPi::CPU::enableFPU();
//    
//    while (1) {
//        
//    }
//}

extern unsigned int __data_start;
extern unsigned int _frame_start;

extern "C"
uint32_t GET32(uint32_t address) {
    return *(uint32_t *)address;
}

extern "C"
void PUT32(uint32_t address, uint32_t value) {
    *(uint32_t*)address = value;
}

extern "C"
void kernel_start(uint32_t r011, uint32_t deviceID, uint32_t ATAGS) {
//    if (!raspberryPi::raspberryPiInit()) {
//        while (1) {
//            asm volatile( "wfi" );
//        }
//    }
    
//    /* No further need to access kernel code at 0x00000000 - 0x000fffff */
//    initpagetable[0] = 0;
//    /* Flush it out of the TLB */
//    asm volatile("mcr p15, 0, %[data], c8, c7, 1" : : [data] "r" (0x00000000));
    
//    raspberryPi::Memory::init();
    
    // 启用这个CPU的FPU
    raspberryPi::CPU::enableFPU();
    
//    raspberryPi::CPU::enableMMU();
    
    // 初始化UART, 默认波特率115200
    raspberryPi::UART::init();
    
    // 初始化Timer, 默认250MHz
    raspberryPi::Timer::init();
    
//    // 设定EMMC时钟,  0x0EE6B280, 250MHz
//    raspberryPi::Clocks::setRate(raspberryPi::Clocks::EMMC,  0x0EE6B280);
//    
//    // 设定UART时钟,  0x3B9ACA00, 1GHz
//    raspberryPi::Clocks::setRate(raspberryPi::Clocks::UART,  0x3B9ACA00);
//    
//    // 设定ARM时钟,   0x35A4E900, 900MHz
//    raspberryPi::Clocks::setRate(raspberryPi::Clocks::ARM,   0x35A4E900);
//    
//    // 设定CORE时钟,  0x0EE6B280, 250MHz
//    raspberryPi::Clocks::setRate(raspberryPi::Clocks::CORE,  0x0EE6B280);
//    
//    // 设定SDRAM时钟, 0x0EE6B280, 450MHz
//    raspberryPi::Clocks::setRate(raspberryPi::Clocks::SDRAM, 0x1AD27480);
//    
//    // 启用USB控制器
//    raspberryPi::Power::enable(raspberryPi::Power::USB_HCD);

    raspberryPi::UART::write("Hello World!\r\n");
    
    // 初始化图形, 640x360, 32bit, 双缓冲
    uint32_t result = raspberryPi::Graphics::init(640, 360, 32, false);
    if (result != 0) {
        // 设定像素颜色顺序
        raspberryPi::Graphics::setPixelOrder(raspberryPi::Graphics::BGR);
        
//        raspberryPi::Graphics::lockFoucs();
        raspberryPi::Graphics::Console::log("Hello World\n");
        
//        // 启用DMA 0
//        raspberryPi::DMA::enableEngine(0);
//        
//        // reset DMA 0
//        raspberryPi::DMA::reset(0);
//        
//        while (1) {
//            uint32_t offset = 0;
//            for (uint32_t frame = 0; frame < 60; frame++) {
//                // 载入图像, 图像在第二framebuffer上
//                // _frame_start
//                raspberryPi::Graphics::loadFrameBuffer((uint32_t *)(0x0000E000) + offset);
//                offset += 230400;
//                raspberryPi::waits(50000);
//                raspberryPi::Graphics::sync();
//            }
//        }
    }
}

//{
//    // 锁定为当前正在显示的framebuffer
//    raspberryPi::Graphics::lockFoucs();
//    
//    // Console输出, 同步显示
//    
//    // 确定EMMC时钟启用
//    if (!raspberryPi::Clocks::enable(raspberryPi::Clocks::EMMC)) {
//        raspberryPi::Graphics::Console::log("Cannot enable EMMC clock\n");
//    } else {
//        raspberryPi::Graphics::Console::log("Enabled EMMC clock\n");
//    }
//    if (raspberryPi::EMMC::isSDCardInserted()) {
//        raspberryPi::Graphics::Console::log("We do have a SD Card on board!\n");
//        if (raspberryPi::EMMC::init()) {
//            raspberryPi::Graphics::Console::log("And we can init it!\n");
//        } else {
//            raspberryPi::Graphics::Console::log("A long day of debug!\n");
//        }
//    } else {
//        raspberryPi::Graphics::Console::log("Fuck EMMC!\n");
//    }
//    raspberryPi::Graphics::Console::log("Anyway, we'll show NAO in 5s...\n");
//    
//    // 解除锁定
//    raspberryPi::Graphics::unlockFoucs();
//}