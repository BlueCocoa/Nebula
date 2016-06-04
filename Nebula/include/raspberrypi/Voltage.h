//
//  Voltage.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/10.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __VLOTAGE_H
#define __VLOTAGE_H

#include <sys/types.h>

namespace raspberryPi {
    namespace Voltage {
        enum voltage_id_t {
            CORE        = 0x00000001,
            SDRAM_C     = 0x00000002,
            SDRAM_P     = 0x00000003,
            SDRAM_I     = 0x00000004,
        };
        enum voltage_t {
            VOLTAGE     = 0x00,
            VOLTAGE_MAX = 0x01,
            VOLTAGE_MIN = 0x02,
        };
        uint32_t voltage(voltage_id_t id, voltage_t voltage = VOLTAGE);
        bool setVoltage(voltage_id_t id, uint32_t value);
        enum temperature_t {
            TEMPERATURE     = 0x00,
            TEMPERATURE_MAX = 0x01
        };
        uint32_t temperature(voltage_id_t id, temperature_t temperature = TEMPERATURE);
    }
}

#endif /* __VLOTAGE_H */
