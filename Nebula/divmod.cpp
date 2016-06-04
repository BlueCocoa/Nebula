//
//  divmod.c
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <sys/types.h>

uint32_t div(uint32_t a, uint32_t b) {
    if (b == 0) return 0;
    uint32_t result = 0;
    while (a >= b) {
        a -= b;
        result++;
    }
    return result;
}

uint32_t mod(uint32_t a, uint32_t b) {
    if (b == 0) return 0;
    while (a >= b) {
        a -= b;
    }
    return a;
}
