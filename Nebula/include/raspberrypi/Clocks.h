//
//  Clocks.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __CLOCKS_H
#define __CLOCKS_H

#include <sys/types.h>

namespace raspberryPi {
    namespace Clocks {
        enum clock_t {
            EMMC        = 0x00000001,
            UART        = 0x00000002,
            ARM         = 0x00000003,
            CORE        = 0x00000004,
            V3D         = 0x00000005,
            H264        = 0x00000006,
            ISP         = 0x00000007,
            SDRAM       = 0x00000008,
            PIXEL       = 0x00000009,
            PWM         = 0x0000000A
        };
        uint32_t state(clock_t clock);
        uint32_t setState(clock_t clock, uint32_t state);
        bool enable(clock_t clock);
        bool disable(clock_t clock);
        enum rate_t {
            RATE     = 0x00,
            RATE_MAX = 0x01,
            RATE_MIN = 0x02
        };
        uint32_t rate(clock_t clock, rate_t rate = RATE);
        uint32_t setRate(clock_t clock, uint32_t clockRate, bool skipSettingTurbo = false);
        uint32_t turbo(clock_t clock);
        bool setTurbo(clock_t clock, uint32_t level);
    }
}

#endif /* __CLOCKS_H */
