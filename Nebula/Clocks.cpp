//
//  Clocks.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/Clocks.h>
#include <raspberrypi/Mailbox.h>

using namespace raspberryPi;
using namespace Mailbox;

uint32_t Clocks::state(clock_t clock) {
    uint32_t params[1] = { 0 };
    params[0] = clock;
    
    volatile uint32_t * result = property(CLOCK_STATE_GET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

uint32_t Clocks::setState(clock_t clock, uint32_t state) {
    uint32_t params[2] = { 0, 0 };
    params[0] = clock;
    params[1] = state;
    
    volatile uint32_t * result = property(CLOCK_STATE_SET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

bool Clocks::enable(clock_t clock) {
    uint32_t result = setState(clock, 0x3);
    if (result & 0x1) return true;
    return false;
}

bool Clocks::disable(clock_t clock) {
    uint32_t result = setState(clock, 0x3);
    if (result & 0x1) return false;
    return true;
}

uint32_t Clocks::rate(clock_t clock, rate_t rate) {
    uint32_t params[1] = { 0 };
    params[0] = clock;
    
    property_t query;
    if (rate == RATE) query = CLOCK_RATE_GET;
    else if (rate == RATE_MAX) query = CLOCK_RATE_MAX;
    else if (rate == RATE_MIN) query = CLOCK_RATE_MIN;
    else return 0xFFFFFFFF;
    
    volatile uint32_t * result = property(query, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

uint32_t Clocks::setRate(clock_t clock, uint32_t clockRate, bool skipSettingTurbo) {
    uint32_t params[3] = { 0, 0, 0 };
    params[0] = clock;
    params[1] = clockRate;
    params[2] = skipSettingTurbo;
    
    volatile uint32_t * result = property(CLOCK_RATE_SET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

uint32_t Clocks::turbo(clock_t clock) {
    uint32_t params[1] = { 0 };
    params[0] = clock;
    
    volatile uint32_t * result = property(TURBO_GET, params);
    if (result[0] != 0) {
        return result[2];
    } else {
        return 0xFFFFFFFF;
    }
}

bool Clocks::setTurbo(clock_t clock, uint32_t level) {
    uint32_t params[2] = { 0, 0 };
    params[0] = clock;
    params[1] = level;
    
    volatile uint32_t * result = property(TURBO_SET, params);
    if (result[0] != 0) {
        return (result[2] == level);
    } else {
        return false;
    }
}
