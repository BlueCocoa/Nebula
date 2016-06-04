//
//  Power.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __POWER_H
#define __POWER_H

#include <sys/types.h>

namespace raspberryPi {
    namespace Power {
        enum device_id_t {
            SD_Card     = 0x00000000,
            UART0       = 0x00000001,
            UART1       = 0x00000002,
            USB_HCD     = 0x00000003,
            I2C0        = 0x00000004,
            I2C1        = 0x00000005,
            I2C2        = 0x00000006,
            SPI         = 0x00000007,
            CCP2TX      = 0x00000008,
        };
        uint32_t state(device_id_t id);
        uint32_t setState(device_id_t id, uint32_t state);
        uint32_t timing(device_id_t id);
        bool enable(device_id_t id, bool wait = true);
        bool disable(device_id_t id, bool wait = true);
    }
}

#endif /* __POWER_H */
