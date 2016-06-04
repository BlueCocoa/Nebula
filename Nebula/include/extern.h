//
//  extern.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/13.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __EXTERN_H
#define __EXTERN_H

#include <sys/types.h>

#define ARM_PERIPHERALS_BASE 0xBF000000

extern uint32_t div(uint32_t a, uint32_t b);
extern uint32_t mod(uint32_t a, uint32_t b);
extern "C" uint32_t GET32(uint32_t address);
extern "C" void PUT32(uint32_t address, uint32_t value);
extern "C" void irq_set();
extern "C" void irq_enable();
extern "C" void os_irq_handler();

#endif /* __EXTERN_H */
