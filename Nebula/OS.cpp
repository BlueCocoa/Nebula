//
//  OS.c
//  Nebula
//
//  Created by BlueCocoa on 15/9/12.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <sys/types.h>
#include <raspberryPi/Graphics.h>

extern "C" void os_irq_handler() {
    raspberryPi::Graphics::Console::log("IRQ called!\n");
}
