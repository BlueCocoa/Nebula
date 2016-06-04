//
//  Mailbox.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/8.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <extern.h>
#include <raspberrypi/Mailbox.h>
#include <raspberrypi/Memory.h>

#include <raspberrypi/UART.h>

using namespace raspberryPi;

static uint32_t RES_TABLE [] = {0, 4, 6, 8, 16, 136, 1024 };
static uint32_t REQ_TABLE [] = {0, 4, 8, 12, 16, 24, 28 };
volatile unsigned int * mailboxBuffer = (unsigned int *) 0x1000;

void Mailbox::write(uint32_t channel, uint32_t value) {
//    if ((((value) << 28) >> 28) != 15) return;
//    if (channel > 15) return;
    
    UART::write("Message received! Send ");
    UART::writeHexString((channel | (value & 0xFFFFFFF0)));
    UART::write("to channel ");
    UART::writeHexStringLine(channel);
    
    while (GET32(MAILBOX_STATUS) & MAIL_FULL) { }
    
    Memory::barrier();
    PUT32(MAILBOX_WRITE, (channel | (value & 0xFFFFFFF0)));
    Memory::barrier();
    
    UART::write("Message Sent!\r\n");
}

uint32_t Mailbox::read(unsigned int channel) {
//    if (channel > 15) return 0;
    UART::write("Reading channel ");
    UART::writeHexStringLine(channel);
    
    unsigned int value;
    while (1) {
        while (GET32(MAILBOX_STATUS) & MAIL_EMPTY) { }
        
        Memory::barrier();
        value = GET32(MAILBOX_BASE);
        Memory::barrier();
        
        if ((value & 0xF) == channel) break;
    }
    
    UART::write("Read as ");
    UART::writeHexStringLine((value & 0xFFFFFFF0));
    
    return (value & 0xFFFFFFF0);
}

volatile uint32_t * Mailbox::property(Mailbox::property_t tag, uint32_t * args) {
    uint8_t requestLengthMask  = (tag & (((1 << 6) - 1) << 20)) >> 20;
    uint8_t responseLengthMask = (tag & (((1 << 6) - 1) << 26)) >> 26;
    uint32_t tagID = (tag << 12) >> 12;
    
    mailboxBuffer[0] = 24 + RES_TABLE[responseLengthMask];
    mailboxBuffer[1] = 0x00000000;
    mailboxBuffer[2] = tagID;
    mailboxBuffer[3] = RES_TABLE[responseLengthMask];
    mailboxBuffer[4] = REQ_TABLE[requestLengthMask];
    
    uint32_t arg;
    for (arg = 0; arg < (REQ_TABLE[requestLengthMask] >> 2); arg++) {
        mailboxBuffer[5 + arg] = args[arg];
    }
    mailboxBuffer[5 + arg] = 0;
    
    write(8, 0x1000);
    read(8);
    
    if (mailboxBuffer[1] != 0x80000000) {
        mailboxBuffer[0] = 0;
        return mailboxBuffer;
    } else {
        mailboxBuffer[4] = (RES_TABLE[responseLengthMask] >> 2);
        return mailboxBuffer + 4;
    }
}
