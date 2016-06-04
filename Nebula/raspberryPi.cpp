//
//  raspberryPi.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/8.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/raspberryPi.h>
#include <raspberrypi/Mailbox.h>

#define ARM_PM_PASSWD		(0x5A << 24)

uint32_t __boardRevision;

bool raspberryPi::raspberryPiInit() {
    bool support = false;
    __boardRevision = ((volatile uint32_t *)raspberryPi::Mailbox::property(raspberryPi::Mailbox::BOARD_REVISION))[1];
    switch (__boardRevision) {
        case 0x00000002:
            model = "Raspberry Pi Model B Revision 1.0";
            memoryBytes = 268435456;
            break;
        case 0x00000003:
            model = "Raspberry Pi Model B Revision 1.0 + ECN0001 (no fuses, D14 removed)";
            memoryBytes = 268435456;
            break;
        case 0x00000004:
        case 0x00000005:
        case 0x00000006:
            model = "Raspberry Pi Model B Revision 2.0 Mounting holes";
            memoryBytes = 268435456;
            break;
        case 0x00000007:
        case 0x00000008:
        case 0x00000009:
            model = "Raspberry Pi Model A Mounting holes";
            memoryBytes = 268435456;
            break;
        case 0x0000000D:
        case 0x0000000E:
        case 0x0000000F:
            model = "Raspberry Pi Model B Revision 2.0 Mounting holes";
            memoryBytes = 536870912;
            break;
        case 0x00000010:
            model = "Raspberry Pi Model B+";
            memoryBytes = 536870912;
            break;
        case 0x00000011:
            model = "Raspberry Pi Compute Module";
            memoryBytes = 536870912;
            break;
        case 0x00000012:
            model = "Raspberry Pi Model A+";
            memoryBytes = 268435456;
            break;
        case 0x00A01041:
            cores = 4;
            model = "Raspberry Pi Pi 2 Model B (Sony, UK)";
            memoryBytes = 1073741824;
            support = true;
            break;
        case 0x00A21041:
            cores = 4;
            model = "Raspberry Pi Pi 2 Model B (Embest, China)";
            memoryBytes = 1073741824;
            support = true;
            break;
        default:
            model = "Unknown Raspberry Pi Model";
            break;
    }
    return support;
}

volatile uint32_t raspberryPi::clock() {
    volatile uint32_t * r1;
    r1 = (uint32_t *)0xF2003004;
    
    uint32_t time = *r1;
    return time;
}

void raspberryPi::waits(uint32_t time) {
    volatile uint32_t * r1;
    r1 = (uint32_t *)0xf2003004;
    
    uint32_t start1 = *r1;
    
    while (1) {
        if (*r1 - start1 > time) {
            break;
        }
    }
}

void raspberryPi::reboot() {
    asm volatile( "dmb" ::: "memory" );
    PUT32(0xF2000000 + 0x100000 + 0x24, ARM_PM_PASSWD | 1);
    PUT32(0xF2000000 + 0x100000 + 0x1C, ARM_PM_PASSWD | 0x20);
    for (;;) ;
}

void raspberryPi::halt() {
    uint32_t MPIDR;
    asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (MPIDR));
    while (1) {
        asm volatile( "dsb" ::: "memory" );
        asm volatile( "wfi" );
    }
}

