//
//  Timer.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <extern.h>
#include <raspberrypi/Timer.h>

using namespace raspberryPi;

void Timer::init() {
    PUT32(ARM_TIMER_CTL, 0x00F90000);
    PUT32(ARM_TIMER_CTL, 0x00F90200);
}

uint32_t Timer::tick() {
    return GET32(ARM_TIMER_CNT);
}
