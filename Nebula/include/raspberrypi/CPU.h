//
//  CPU.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __CPU_H
#define __CPU_H

#include <sys/types.h>

#define L1_DATA_CACHE_SETS      128
#define L1_SETWAY_SET_SHIFT     6
#define L1_DATA_CACHE_WAYS      4
#define L1_SETWAY_WAY_SHIFT     30
#define SETWAY_LEVEL_SHIFT      1

namespace raspberryPi {
    namespace CPU {
        volatile uint32_t CPUID();
        volatile void enableL1DataCache();
        volatile void enableFPU();
        volatile void enableMMU();
        volatile void MMU(bool enable);
        volatile void enableCore(uint32_t core);
        volatile void enableMultiCore();
        uint32_t setClockRate(uint32_t clockRate = 0x35A4E900, bool skipSettingTurbo = false);
        uint32_t clockRate();
        volatile void invalidateL1DataCache();
    }
}

#endif /* __CPU_H */
