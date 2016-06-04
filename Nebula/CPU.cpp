//
//  CPU.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/CPU.h>
#include <raspberrypi/Mailbox.h>

using namespace raspberryPi;

extern "C" void kernel_start_slave();

volatile uint32_t CPU::CPUID() {
    // Multiprocessor Affinity Register
    uint32_t MPIDR;
    asm volatile( "mrc p15, 0, %0, c0, c0, 5" : "=r" (MPIDR));
    return (MPIDR & 3);
}

volatile void CPU::enableL1DataCache() {
    // System Control Register
    uint32_t SCR;
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : "=r" (SCR) );
    
    // Data Cache
    SCR |= 0x0004;
    // Branch Prediction
    SCR |= 0x0800;
    // Instruction Caches
    SCR |= 0x1000;
    
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : : "r" (SCR) : "memory");
}

volatile void CPU::enableFPU() {
    // Coprocessor Access Control Register
    unsigned CACR;
    asm volatile( "mrc p15, 0, %0, c1, c0, 2" : "=r" (CACR) );
    
    // 启用单精度
    CACR |= 3 << 20;
    // 启用双精度
    CACR |= 3 << 22;
    
    asm volatile( "mcr p15, 0, %0, c1, c0, 2" : : "r" (CACR) : "memory");
    
    // 启用VFP
    asm volatile( "fmxr fpexc, %0" : : "r" ((1 << 30)) );
    asm volatile( "fmxr fpscr, %0" : : "r" (0) );
}

volatile void CPU::enableMMU() {
    static volatile __attribute__ ((aligned (0x4000))) unsigned PageTable[4096];
    
    unsigned base;
    for (base = 0; base < 1024-16; base++)
    {
        // section descriptor (1 MB)
        // outer and inner write back, write allocate, not shareable (fast but unsafe)
        //PageTable[base] = base << 20 | 0x0140E;
        // outer and inner write through, no write allocate, shareable (safe but slower)
        PageTable[base] = base << 20 | 0x1040A;
    }
    for (; base < 4096; base++)
    {
        // shared device, never execute
        PageTable[base] = base << 20 | 0x10416;
    }
    
    // set SMP bit in ACTLR
    unsigned auxctrl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
    auxctrl |= 1 << 6;
    asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));
    
    // set domain 0 to client
    asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (1));
    
    // always use TTBR0
    asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));
    
    // set TTBR0 (page table walk inner and outer non-cacheable, non-shareable memory)
    asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (0 | (unsigned) &PageTable));
    
    asm volatile ("isb" ::: "memory");
    
    // enable MMU
    unsigned SCR;
    asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (SCR));
    SCR |= 0x1;
    asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r" (SCR) : "memory");
}

volatile void CPU::MMU(bool enable) {
    unsigned SCR;
    asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r" (SCR));
    
    if (enable) {
        SCR |= 0x1;
    } else {
        SCR &= ~(0x1);
    }
    
    asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r" (SCR) : "memory");
}

volatile void CPU::enableCore(uint32_t core) {
    asm volatile( "dmb" ::: "memory" );
    if (core == 0) return;
    
    // MPIDR
    uint32_t MPIDR;
    asm volatile( "mrc p15, 0, %0, c0, c0, 5" : "=r" (MPIDR));
    MPIDR >>= 31;
}

volatile void CPU::enableMultiCore() {
//    for (unsigned nCore = 1; nCore < 4; nCore++)
//    {
//        uint32_t nMailBoxClear = (0xC0000000 + 0x00CC) + 0x10 * nCore;
//        
//        asm volatile( "dsb" ::: "memory");
//        
//        unsigned nTimeout = 100;
//        while (GET32 (nMailBoxClear) != 0)
//        {
//            if (--nTimeout == 0)
//            {
//                return ;
//            }
//            
//            raspberryPi::waits(1);
//        }
//        
//        PUT32 ((0xC0000000 + 0x008C) + 0x10 * nCore, (uint32_t) &_start_secondary);
//        
//        nTimeout = 100;
//        while (GET32 (nMailBoxClear) != 0)
//        {
//            if (--nTimeout == 0)
//            {
//                
//                return ;
//            }
//            raspberryPi::waits(1);
//        }
//    }
}

uint32_t CPU::setClockRate(uint32_t clockRate, bool skipSettingTurbo) {
    uint32_t rateParams[3] = { 0x000000003, 0x35A4E900, 0 };
    rateParams[1] = clockRate;
    rateParams[2] = skipSettingTurbo;
    volatile uint32_t * result = Mailbox::property(Mailbox::CLOCK_RATE_SET, rateParams);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

uint32_t CPU::clockRate() {
    volatile uint32_t * result = Mailbox::property(Mailbox::CLOCK_RATE_GET);
    if (result[0] != 0) {
        return result[1];
    } else {
        return 0xFFFFFFFF;
    }
}

volatile void CPU::invalidateL1DataCache() {
    for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++) {
        for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++) {
            register uint32_t nSetWayLevel = nWay << L1_SETWAY_WAY_SHIFT
            | nSet << L1_SETWAY_SET_SHIFT
            | 0 << SETWAY_LEVEL_SHIFT;
        }
    }
}
