//
//  string.c
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include "string.h"

uint32_t strlen(const char * str) {
    unsigned int length;
    length = 0;
    for (;;length++) {
        if (str[length] == '\0') break;
    }
    return length;
}
