//
//  UART.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/raspberryPi.h>
#include <raspberrypi/UART.h>
#include <raspberrypi/GPIO.h>
#include "string.h"

using namespace raspberryPi;

void UART::init(uint32_t baudrate) {
    unsigned int ra;
    PUT32(AUX_ENABLES, 1);
    PUT32(AUX_MU_IER_REG, 0);
    PUT32(AUX_MU_CNTL_REG, 0);
    PUT32(AUX_MU_LCR_REG, 3);
    PUT32(AUX_MU_MCR_REG, 0);
    PUT32(AUX_MU_IER_REG, 0);
    PUT32(AUX_MU_IIR_REG, 0xC6);
    PUT32(AUX_MU_BAUD_REG, baudrate);
    ra = GET32(GPFSEL1);
    ra &= ~(7 << 12);
    ra |= 2 << 12;
    ra &= ~(7 << 15);
    ra |= 2 << 15;
    PUT32(GPFSEL1, ra);
    PUT32(GPPUD, 0);
    for(ra = 0; ra < 150; ra++) wait_op();
    PUT32(GPPUDCLK0, (1 << 14) | (1 << 15));
    for(ra = 0; ra < 150; ra++) wait_op();
    PUT32(GPPUDCLK0, 0);
    PUT32(AUX_MU_CNTL_REG, 3);
}

bool UART::check() {
    if(GET32(AUX_MU_LSR_REG) & 0x01) return YES;
    return NO;
}

uint32_t UART::readByte() {
    while(1)
        if(GET32(AUX_MU_LSR_REG) & 0x01) break;
    return(GET32(AUX_MU_IO_REG) & 0xFF);
}

void UART::readUntil(char *str, char terminator) {
    unsigned int length = 0;
    unsigned int byte = readByte();
    while (byte != terminator) {
        str[length] = byte;
        length++;
        byte = readByte();
    }
    str[length] = '\0';
}

void UART::readLine(char *str) {
    readUntil(str, '\n');
}

void UART::writeByte(uint32_t byte) {
    while(1)
        if(GET32(AUX_MU_LSR_REG) & 0x20) break;
    PUT32(AUX_MU_IO_REG, byte);
}

void UART::write(const char *str) {
    unsigned int pos = 0;
    for (; pos < strlen(str); pos++)
        writeByte(str[pos]);
}

void UART::writeLine(const char *str) {
    write(str);
    writeByte('\r');
    writeByte('\n');
}

void UART::writeByAddress(uint32_t address) {
    unsigned int flag;
    unsigned int byte_pos;
    union {
        unsigned int raw;
        char byte[4];
    } data;
    
    flag = 0;
    byte_pos = 0;
    
    while (1) {
        data.raw = GET32(address);
        while (1) {
            if (data.byte[byte_pos] == '\0') {
                flag = 1;
                break;
            }
            writeByte(data.byte[byte_pos]);
            byte_pos++;
            if (byte_pos == 4) {
                byte_pos = 0;
                break;
            }
        }
        if (flag == 1) break;
        address += 4;
    }
    writeByte('\r');
    writeByte('\n');
}

void UART::writeHexString(uint32_t data) {
    unsigned int rb;
    unsigned int rc;
    rb = 32;
    while(1) {
        rb -= 4;
        rc = (data >> rb) & 0xF;
        if(rc > 9) rc += 0x37;
        else rc += 0x30;
        writeByte(rc);
        if(rb == 0) break;
    }
    writeByte(0x20);
}

void UART::writeHexStringLine(uint32_t data) {
    writeHexString(data);
    writeByte('\r');
    writeByte('\n');
}

