//
//  raspberryPi.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __REASPBERRYPI_H
#define __REASPBERRYPI_H

#include <architecture/arch.h>
#include <extern.h>
#include <sys/types.h>

namespace raspberryPi {
    /**
     *  @brief  树莓派型号
     */
    static const char * model;
    
    /**
     *  @brief  核心数量
     */
    static uint32_t cores = 1;
    
    /**
     *  @brief  内存大小
     */
    static uint64_t memoryBytes = 0;
    
    /**
     *  @brief  设备ID
     */
    static uint32_t deviceID   = 0;
    
    /**
     *  @brief  ATAGS开始地址
     */
    static uint32_t ATAGS      = 0;
    
    /**
     *  @brief  总线地址
     */
    static uint32_t BusAddress = 0x80000000;
    
    static cpu_arch_t arch;
    static cpu_subarch_t subarch;
    
    bool raspberryPiInit();
    
    /**
     *  @brief  等待一个CPU操作时间
     */
    extern "C" void wait_op();
    
    volatile uint32_t clock();
    
    /**
     *  @brief  等待微秒
     */
    void waits(uint32_t microseconds);
    
    /**
     *  @brief  重启树莓派
     */
    void reboot();
    
    /**
     *  @brief  halt
     */
    void halt();
}

#endif /* __REASPBERRYPI_H */
