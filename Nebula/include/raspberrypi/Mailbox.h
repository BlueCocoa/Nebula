//
//  Mailbox.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __MAILBOX_H
#define __MAILBOX_H

#include <sys/types.h>

#pragma mark
#pragma mark - Definitions

#define MAILBOX_BASE                (ARM_PERIPHERALS_BASE + 0xB880)
#define MAILBOX_STATUS              (MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE               (MAILBOX_BASE + 0x20)
#define MAIL_FULL                   0x80000000
#define MAIL_EMPTY                  0x40000000

//                                Bit |6bit|   25 ... 20|   19 ... 0
#define TAG_RES_LEN_0               0b00000000000000000000000000000000
#define TAG_RES_LEN_4               0b00000100000000000000000000000000
#define TAG_RES_LEN_6               0b00001000000000000000000000000000
#define TAG_RES_LEN_8               0b00001100000000000000000000000000
#define TAG_RES_LEN_16              0b00010000000000000000000000000000
#define TAG_RES_LEN_136             0b00010100000000000000000000000000
#define TAG_RES_LEN_1024            0b00011000000000000000000000000000
#define TAG_RES_LEN_V               0b11111100000000000000000000000000

//                 Bit |      31 ... 26     |6bit|   19 ... 0
#define TAG_REQ_LEN_0               0b00000000000000000000000000000000
#define TAG_REQ_LEN_4               0b00000000000100000000000000000000
#define TAG_REQ_LEN_8               0b00000000001000000000000000000000
#define TAG_REQ_LEN_12              0b00000000001100000000000000000000
#define TAG_REQ_LEN_16              0b00000000010000000000000000000000
#define TAG_REQ_LEN_24              0b00000000010100000000000000000000
#define TAG_REQ_LEN_28              0b00000000011000000000000000000000
#define TAG_REQ_LEN_V               0b00000011111100000000000000000000

#define TAG_FIRMWARE_REVISION       0x00000001
#define TAG_BOARD_MODEL             0x00010001
#define TAG_BOARD_REVSION           0x00010002
#define TAG_BOARD_MAC               0x00010003
#define TAG_BOARD_SERIAL            0x00010004
#define TAG_ARM_MEMORY              0x00010005
#define TAG_VIDEOCORE_MEMORY        0x00010006
#define TAG_CLOCKS                  0x00010007
#define TAG_COMMAND_LINE            0x00050001
#define TAG_DMA_CHANNEL             0x00060001
#define TAG_POWER_STATE_GET         0x00020001
#define TAG_TIMING                  0x00020002
#define TAG_POWER_STATE_SET         0x00028001
#define TAG_CLOCK_STATE_GET         0x00030001
#define TAG_CLOCK_STATE_SET         0x00038001
#define TAG_CLOCK_RATE_GET          0x00030002
#define TAG_CLOCK_RATE_SET          0x00038002
#define TAG_CLOCK_RATE_MAX          0x00030004
#define TAG_CLOCK_RATE_MIN          0x00030007
#define TAG_TURBO_GET               0x00030009
#define TAG_TURBO_SET               0x00038009
#define TAG_VOLTAGE_GET             0x00030003
#define TAG_VOLTAGE_SET             0x00038003
#define TAG_VOLTAGE_MAX             0x00030005
#define TAG_VOLTAGE_MIN             0x00030008
#define TAG_TEMPERATURE             0x00030006
#define TAG_TEMPERATURE_MAX         0x0003000A
#define TAG_MEMORY_ALLOCATE         0x0003000C
#define TAG_MEMORY_LOCK             0x0003000D
#define TAG_MEMORY_UNLOCK           0x0003000E
#define TAG_MEMORY_RELEASE          0x0003000F
#define TAG_EXECUTE_CODE            0x00030010
#define TAG_DISPMANX_RESOURCE       0x00030014
#define TAG_EDID_BLOCK              0x00030020
#define TAG_FRAMEBUFFER_ALLOCATE    0x00040001
#define TAG_FRAMEBUFFER_RELEASE     0x00048001
#define TAG_BLANK_SCREEN            0x00040002
#define TAG_PHYSICAL_SIZE_GET       0x00040003
#define TAG_PHYSICAL_SIZE_TEST      0x00044003
#define TAG_PHYSICAL_SIZE_SET       0x00048003
#define TAG_VIRTUAL_SIZE_GET        0x00040004
#define TAG_VIRTUAL_SIZE_TEST       0x00044004
#define TAG_VIRTUAL_SIZE_SET        0x00048004
#define TAG_DEPTH_GET               0x00040005
#define TAG_DEPTH_TEST              0x00044005
#define TAG_DEPTH_SET               0x00048005
#define TAG_PIXEL_ORDER_GET         0x00040006
#define TAG_PIXEL_ORDER_TEST        0x00044006
#define TAG_PIXEL_ORDER_SET         0x00048006
#define TAG_ALPHA_MODE_GET          0x00040007
#define TAG_ALPHA_MODE_TEST         0x00044007
#define TAG_ALPHA_MODE_SET          0x00048007
#define TAG_PITCH                   0x00040008
#define TAG_VIRTUAL_OFFSET_GET      0x00040009
#define TAG_VIRTUAL_OFFSET_TEST     0x00044009
#define TAG_VIRTUAL_OFFSET_SET      0x00048009
#define TAG_OVERSCAN_GET            0x0004000A
#define TAG_OVERSCAN_TEST           0x0004400A
#define TAG_OVERSCAN_SET            0x0004800A
#define TAG_PALETTE_GET             0x0004000B
#define TAG_PALETTE_TEST            0x0004400B
#define TAG_PALETTE_SET             0x0004800B
#define TAG_CURSOR_INFO_SET         0x00008011
#define TAG_CURSOR_STATE_SET        0x00008010

namespace raspberryPi {
    namespace Mailbox {
        void write(uint32_t channel, uint32_t value);
        uint32_t read(uint32_t channel);
        enum property_t {
            //                  Bit |      31 ... 26     |     25 ... 20    |   19 ... 0
            //                      |     Res Length     |    Req Length    |      Tag
            FIRMWARE_REV            =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_FIRMWARE_REVISION),
            BOARD_MODEL             =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_BOARD_MODEL),
            BOARD_REVISION          =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_BOARD_REVSION),
            BOARD_MAC               =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_BOARD_MAC),
            BOARD_SERIAL            =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_BOARD_SERIAL),
            ARM_MEMORY              =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_ARM_MEMORY),
            VIDEOCORE_MEMORY        =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_VIDEOCORE_MEMORY),
            CLOCKS                  =  (TAG_RES_LEN_V    | TAG_REQ_LEN_0    | TAG_CLOCKS),
            COMMAND_LINE            =  (TAG_RES_LEN_V    | TAG_REQ_LEN_0    | TAG_COMMAND_LINE),
            DMA_CHANNEL             =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_DMA_CHANNEL),
            POWER_STATE_GET         =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_POWER_STATE_GET),
            TIMING                  =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_TIMING),
            POWER_STATE_SET         =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_POWER_STATE_SET),
            CLOCK_STATE_GET         =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_CLOCK_STATE_GET),
            CLOCK_STATE_SET         =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_CLOCK_STATE_SET),
            CLOCK_RATE_GET          =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_CLOCK_RATE_GET),
            CLOCK_RATE_SET          =  (TAG_RES_LEN_8    | TAG_REQ_LEN_12   | TAG_CLOCK_RATE_SET),
            CLOCK_RATE_MAX          =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_CLOCK_RATE_MAX),
            CLOCK_RATE_MIN          =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_CLOCK_RATE_MIN),
            TURBO_GET               =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_TURBO_GET),
            TURBO_SET               =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_TURBO_SET),
            VOLTAGE_GET             =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_VOLTAGE_GET),
            VOLTAGE_SET             =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_VOLTAGE_SET),
            VOLTAGE_MAX             =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_VOLTAGE_MAX),
            VOLTAGE_MIN             =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_VOLTAGE_MIN),
            TEMPERATURE             =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_TEMPERATURE),
            TEMPERATURE_MAX         =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_TEMPERATURE_MAX),
            MEMORY_ALLOCATE         =  (TAG_RES_LEN_4    | TAG_REQ_LEN_12   | TAG_MEMORY_ALLOCATE),
            MEMORY_LOCK             =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_MEMORY_LOCK),
            MEMORY_UNLOCK           =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_MEMORY_UNLOCK),
            MEMORY_RELEASE          =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_MEMORY_RELEASE),
            EXECUTE_CODE            =  (TAG_RES_LEN_4    | TAG_REQ_LEN_28   | TAG_EXECUTE_CODE),
            DISPMANX_RESOURCE       =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_DISPMANX_RESOURCE),
            EDID_BLOCK              =  (TAG_RES_LEN_136  | TAG_REQ_LEN_4    | TAG_EDID_BLOCK),
            FRAMEBUFFER_ALLOCATE    =  (TAG_RES_LEN_8    | TAG_REQ_LEN_4    | TAG_FRAMEBUFFER_ALLOCATE),
            FRAMEBUFFER_RELEASE     =  (TAG_RES_LEN_0    | TAG_REQ_LEN_0    | TAG_FRAMEBUFFER_RELEASE),
            BLANK_SCREEN            =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_BLANK_SCREEN),
            PHYSICAL_SIZE_GET       =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_PHYSICAL_SIZE_GET),
            PHYSICAL_SIZE_TEST      =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_PHYSICAL_SIZE_TEST),
            PHYSICAL_SIZE_SET       =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_PHYSICAL_SIZE_SET),
            VIRTUAL_SIZE_GET        =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_VIRTUAL_SIZE_GET),
            VIRTUAL_SIZE_TEST       =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_VIRTUAL_SIZE_TEST),
            VIRTUAL_SIZE_SET        =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_VIRTUAL_SIZE_SET),
            DEPTH_GET               =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_DEPTH_GET),
            DEPTH_TEST              =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_DEPTH_TEST),
            DEPTH_SET               =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_DEPTH_SET),
            PIXEL_ORDER_GET         =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_PIXEL_ORDER_GET),
            PIXEL_ORDER_TEST        =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_PIXEL_ORDER_TEST),
            PIXEL_ORDER_SET         =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_PIXEL_ORDER_SET),
            ALPHA_MODE_GET          =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_ALPHA_MODE_GET),
            ALPHA_MODE_TEST         =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_ALPHA_MODE_TEST),
            ALPHA_MODE_SET          =  (TAG_RES_LEN_4    | TAG_REQ_LEN_4    | TAG_ALPHA_MODE_SET),
            PITCH                   =  (TAG_RES_LEN_4    | TAG_REQ_LEN_0    | TAG_PITCH),
            VIRTUAL_OFFSET_GET      =  (TAG_RES_LEN_8    | TAG_REQ_LEN_0    | TAG_VIRTUAL_OFFSET_GET),
            VIRTUAL_OFFSET_TEST     =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_VIRTUAL_OFFSET_TEST),
            VIRTUAL_OFFSET_SET      =  (TAG_RES_LEN_8    | TAG_REQ_LEN_8    | TAG_VIRTUAL_OFFSET_SET),
            OVERSCAN_GET            =  (TAG_RES_LEN_16   | TAG_REQ_LEN_0    | TAG_OVERSCAN_GET),
            OVERSCAN_TEST           =  (TAG_RES_LEN_16   | TAG_REQ_LEN_16   | TAG_OVERSCAN_TEST),
            OVERSCAN_SET            =  (TAG_RES_LEN_16   | TAG_REQ_LEN_16   | TAG_OVERSCAN_SET),
            PALETTE_GET             =  (TAG_RES_LEN_1024 | TAG_REQ_LEN_0    | TAG_PALETTE_GET),
            PALETTE_TEST            =  (TAG_RES_LEN_4    | TAG_REQ_LEN_V    | TAG_PALETTE_TEST),
            PALETTE_SET             =  (TAG_RES_LEN_4    | TAG_REQ_LEN_V    | TAG_PALETTE_SET),
            CURSOR_INFO_SET         =  (TAG_RES_LEN_4    | TAG_REQ_LEN_24   | TAG_CURSOR_INFO_SET),
            CURSOR_STATE_SET        =  (TAG_RES_LEN_4    | TAG_REQ_LEN_16   | TAG_CURSOR_STATE_SET),
        };
        volatile uint32_t * property(property_t tag, uint32_t * args = 0);
    }
}

#endif /* __MAILBOX_H */
