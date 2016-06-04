//
//  Power.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/8.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/Power.h>
#include <raspberrypi/Mailbox.h>

using namespace raspberryPi;
using namespace Mailbox;

uint32_t Power::state(device_id_t id) {
    uint32_t params[1] = { 0 };
    params[0] = id;
    
    volatile uint32_t * result = property(POWER_STATE_GET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
    return 0;
}

uint32_t Power::setState(device_id_t id, uint32_t state) {
    uint32_t params[2] = { 0, 0 };
    params[0] = id;
    params[1] = state;
    
    volatile uint32_t * result = property(TIMING, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
    return 0;
}

uint32_t Power::timing(device_id_t id) {
    uint32_t params[1] = { 0 };
    params[0] = id;
    
    volatile uint32_t * result = property(TIMING, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
    return 0;
}

bool Power::enable(device_id_t id, bool wait) {
    uint32_t params[2] = { 0, 0 };
    params[0] = id;

    if (wait) params[1] = 0x3;
    else params[1] = 0x1;
    
    volatile uint32_t * result = property(POWER_STATE_SET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
    return 0;
}

bool Power::disable(device_id_t id, bool wait) {
    uint32_t params[2] = { 0, 0 };
    params[0] = id;
    
    if (wait) params[1] = 0x3;
    else params[1] = 0x1;
    
    volatile uint32_t * result = property(POWER_STATE_SET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
    return 0;
}
