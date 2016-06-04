//
//  Voltage.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/10.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/Voltage.h>
#include <raspberrypi/Mailbox.h>

using namespace raspberryPi;
using namespace Mailbox;

uint32_t Voltage::voltage(Voltage::voltage_id_t id, Voltage::voltage_t voltage) {
    uint32_t params[1] = { 0 };
    params[0] = id;
    
    property_t query;
    if (voltage == VOLTAGE) query = VOLTAGE_GET;
    else if (voltage == VOLTAGE_MAX) query = Mailbox::VOLTAGE_MAX;
    else if (voltage == VOLTAGE_MIN) query = Mailbox::VOLTAGE_MIN;
    else return 0xFFFFFFFF;
    
    volatile uint32_t * result = property(query, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

bool Voltage::setVoltage(Voltage::voltage_id_t id, uint32_t value) {
    uint32_t params[2] = { 0, 0 };
    params[0] = id;
    params[1] = value;
    
    volatile uint32_t * result = property(VOLTAGE_SET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

uint32_t Voltage::temperature(voltage_id_t id, temperature_t temperature) {
    uint32_t params[1] = { 0 };
    params[0] = id;
    
    property_t query;
    if (temperature == TEMPERATURE) query = Mailbox::TEMPERATURE;
    else if (temperature == TEMPERATURE_MAX) query = Mailbox::TEMPERATURE_MAX;
    else return 0xFFFFFFFF;
    
    volatile uint32_t * result = property(query, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}
