//
//  UART.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __UART_H
#define __UART_H

#include <sys/types.h>

#pragma mark
#pragma mark - Definitions

#define AUX_BASE            (ARM_PERIPHERALS_BASE + 0x215000)
#define AUX_IRQ             (AUX_BASE + 0x00)
#define AUX_ENABLES         (AUX_BASE + 0x04)
#define AUX_MU_IO_REG       (AUX_BASE + 0x40)
#define AUX_MU_IER_REG      (AUX_BASE + 0x44)
#define AUX_MU_IIR_REG      (AUX_BASE + 0x48)
#define AUX_MU_LCR_REG      (AUX_BASE + 0x4C)
#define AUX_MU_MCR_REG      (AUX_BASE + 0x50)
#define AUX_MU_LSR_REG      (AUX_BASE + 0x54)
#define AUX_MU_MSR_REG      (AUX_BASE + 0x58)
#define AUX_MU_SCRATCH      (AUX_BASE + 0x5C)
#define AUX_MU_CNTL_REG     (AUX_BASE + 0x60)
#define AUX_MU_STAT_REG     (AUX_BASE + 0x64)
#define AUX_MU_BAUD_REG     (AUX_BASE + 0x68)
#define AUX_SPI0_CNTL0_REG  (AUX_BASE + 0x80)
#define AUX_SPI0_CNTL1_REG  (AUX_BASE + 0x84)
#define AUX_SPI0_STAT_REG   (AUX_BASE + 0x88)
#define AUX_SPI0_IO_REG     (AUX_BASE + 0x90)
#define AUX_SPI0_PEEK_REG   (AUX_BASE + 0x94)
#define AUX_SPI1_CNTL0_REG  (AUX_BASE + 0xC0)
#define AUX_SPI1_CNTL1_REG  (AUX_BASE + 0xC4)
#define AUX_SPI1_STAT_REG   (AUX_BASE + 0xC8)
#define AUX_SPI1_IO_REG     (AUX_BASE + 0xD0)
#define AUX_SPI1_PEEK_REG   (AUX_BASE + 0xD4)

namespace raspberryPi {
    namespace UART {
        enum {
            BAUD_115200 = 270,
            BAUD_57600  = 541,
            BAUD_19200  = 1626,
            BAUD_9600   = 3254,
            BAUD_4800   = 6509,
        };
        void init(uint32_t baudrate = BAUD_115200);
        bool check();
        uint32_t readByte();
        void readUntil(char * str, char terminator);
        void readLine(char * str);
        void writeByte(uint32_t byte);
        void write(const char * str);
        void writeLine(const char * str);
        void writeByAddress(uint32_t address);
        void writeHexString(uint32_t data);
        void writeHexStringLine(uint32_t data);
    }
}

#endif /* __UART_H */
