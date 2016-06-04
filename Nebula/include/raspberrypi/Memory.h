//
//  Memory.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __MEMORY_H
#define __MEMORY_H

#include <sys/types.h>

namespace raspberryPi {
    namespace Memory {
        volatile void barrier();
        void init();
        uint32_t addressToPhysical(uint32_t addr);
        uint32_t addressToVirtual(uint32_t addr);
        void memclr(void * address, uint32_t length);
        void * memmove(void * dest, const void * src, uint32_t length);
        void memcpy(uint32_t * dest, uint32_t * src, uint32_t length);
        
    }
}

#endif /* __MEMORY_H */
