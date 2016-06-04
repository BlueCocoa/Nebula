//
//  GPIO.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <extern.h>
#include <sys/types.h>
#include <raspberrypi/GPIO.h>

using namespace raspberryPi;

void GPIO::setFunction(uint32_t pin, function_t function) {
    if (pin > 53) return;
    if (function > 7) return;
    
    unsigned int gpio_addr = GPIO_BASE;
    while (pin > 9) {
        pin -= 10;
        gpio_addr += 4;
    }
    pin = (pin << 1) + pin;
    uint32_t functionMask = function << pin;
    
    unsigned int mask = 7 << pin;
    unsigned int o_mask = GET32(gpio_addr);
    mask = ~mask;
    o_mask &= mask;
    o_mask |= functionMask;
    PUT32(gpio_addr, o_mask);
}

void GPIO::set(uint32_t pin, bool on) {
    if (pin > 53) return;
    
    uint32_t gpioAddress = GPIO_BASE;
    uint32_t pinBank = (pin >> 5) << 2;
    gpioAddress += pinBank;
    
    if (on) gpioAddress += 0x1C;
    else    gpioAddress += 0x28;
    
    uint32_t oMask = GET32(gpioAddress);
    oMask |= 1 << (pin & 31);
    PUT32(gpioAddress, oMask);
}

void GPIO::pull(uint32_t pin, pull_t pull, bool on) {
    if (pin > 53) return;
    
    PUT32(GPPUD, pull);
    
    uint32_t gpioAddress = GPPUDCLK0;
    unsigned int pinBank = (pin >> 5) << 2;
    gpioAddress += pinBank;
    
    uint32_t oMask = GET32(gpioAddress);
    oMask |= (on ? 1 : 0) << pin;
    PUT32(gpioAddress, oMask);
}

void GPIO::detectEvent(uint32_t pin, GPIO_event_t event) {
    if (pin > 53) return;
    
    uint32_t gpioAddress = GPIO_BASE + event;
    uint32_t pinBank = (pin >> 5) << 2;
    gpioAddress += pinBank;
    
    uint32_t oMask = GET32(gpioAddress);
    oMask |= 1 << (pin & 31);
    PUT32(gpioAddress, oMask);
}

bool GPIO::level(uint32_t pin) {
    if (pin > 53) return false;
    
    uint32_t gpioAddress = GPLEV0;
    uint32_t pinBank = (pin >> 5) << 2;
    gpioAddress += pinBank;
    
    return ((GET32(gpioAddress) & (1 << (pin & 31))) == 0);
}
