//
//  EMMC.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __EMMC_H
#define __EMMC_H

#include <sys/types.h>

#pragma mark
#pragma mark - Definitions

#define EMMC_BASE               (ARM_PERIPHERALS_BASE + 0x300000)

/**
 *  @brief  ACMD23 Argument
 *
 *  @discussion This register contains the argument for the SD card specific command ACMD23 (SET_WR_BLK_ERASE_COUNT). ARG2 must be set before the ACMD23 command is issued using the CMDTM register.
 *
 *   Bit(s)   R/W       Reset    Field Name            Description
 *   31:0      R        0x0      ARGUMENT              Argument to be issued with ACMD23
 */
#define EMMC_ARG2               (EMMC_BASE + 0x00)

/**
 *  @brief  Block Size and Count
 *
 *  @discussion This register must not be accessed or modified while any data transfer between card and host is ongoing.
 *              It contains the number and size in bytes for data blocks to be transferred. Please note that the EMMC module restricts the maximum block size to the size of the internal data FIFO which is 1k bytes.
 *              BLKCNT is used to tell the host how many blocks of data are to be transferred. Once the data transfer has started and the TM_BLKCNT_EN bit in the CMDTM register is set the EMMC module automatically decreases the BNTCNT value as the data blocks are transferred and stops the transfer once BLKCNT reaches 0.
 *
 *   Bit(s)   R/W       Reset    Field Name            Description
 *   31:16    R/W        0x0     BLKCNT                Number of blocks to be transferred
 *   15:10                                             Reserved - Write as 0, read as don't care
 *    9:0     R/W        0x0     BLKSIZE               Block size in bytes
 */
#define EMMC_BLKSIZECNT         (EMMC_BASE + 0x04)

/**
 *  @brief  Argument
 *
 *  @discussion This register contains the arguments for all commands except for the SD card specific command ACMD23 which uses ARG2. ARG1 must be set before the command is issued using the CMDTM register.
 *
 *   Bit(s)   R/W       Reset    Field Name            Description
 *   31:0      R        0x0      ARGUMENT              Argument to be issued with command
 */
#define EMMC_ARG1               (EMMC_BASE + 0x08)

/**
 *  @brief  Command and Transfer Mode
 *
 *  @discussion This register is used to issue commands to the card. Besides the command it also contains flags informing the EMMC module what card response and type of data transfer to expect. Incorrect flags will result in strange behaviour.
 *              For data transfers two modes are supported: either transferring a single block of data or several blocks of the same size. The SD card uses two different sets of commands to differentiate between them but the host needs to be additionally configured using TM_MULTI_BLOCK. It is important that this bit is set correct for the command sent to the card, i.e. 1 for CMD18 and CMD25 and 0 for CMD17 and CMD24. Multiple block transfer gives a better performance.
 *              The BLKSIZECNT register is used to configure the size and number of blocks to be transferred. If bit TM_BLKCNT_EN of this register is set the transfer stops automatically after the number of data blocks configured in the BLKSIZECNT register has been transferred.
 *              The TM_AUTO_CMD_EN bits can be used to make the host to send automatically a command to the card telling it that the data transfer has finished once the BLKCNT bits in the BLKSIZECNT register are 0.
 *
 *   Bit(s)   R/W       Reset    Field Name            Description
 *   31:30                                             Reserved - Write as 0, read as don't care
 *   29:24    R/W       0x0      CMD_INDEX             Index of the command to be issued to the card
 *   23:22    R/W       0x0      CMD_TYPE              Type of command to be issued to the card:
 *                                                     00 = normal
 *                                                     01 = suspend (the current data transfer)
 *                                                     10 = resume (the last data transfer)
 *                                                     11 = abort (the current data transfer)
 *     21     R/W       0x0      CMD_ISDATA            Command involves data transfer:
 *                                                     0 = no data transfer command
 *                                                     1 = data transfer command
 *     20     R/W       0x0      CMD_IXCHK_EN          Check that response has same index as command:
 *                                                     0 = disable
 *                                                     1 = enable
 *     19     R/W       0x0      CMD_CRCCHK_EN         Check the responses CRC:
 *                                                     0 = disable
 *                                                     1 = enable
 *     18                                              Reserved - Write as 0, read as don't care
 *   17:16    R/W       0x0      CMD_RSPNS_TYPE        Type of expected response from card:
 *                                                     00 = no response
 *                                                     01 = 136 bits response
 *                                                     10 = 48 bits response
 *                                                     11 = 48 bits response using busy
 *   15:6                                              Reserved - Write as 0, read as don't care
 *     5      R/W       0x0      TM_MULTI_BLOCK        Type of data transfer
 *                                                     0 = single block
 *                                                     1 = multiple block
 *     4      R/W       0x0      TM_DAT_DIR            Direction of data transfer:
 *                                                     0 = from host to card
 *                                                     1 = from card to host
 *    3:2     R/W       0x0      TM_AUTO_CMD_EN        Select the command to be send after completion of a data transfer:
 *                                                     00 = no command
 *                                                     01 = command CMD12
 *                                                     10 = command CMD23
 *                                                     11 = reserved
 *     1      R/W       0x0      TM_BLKCNT_EN          Enable the block counter for multiple block transfers:
 *                                                     0 = disable
 *                                                     1 = enable
 *     0                                               Reserved - Write as 0, read as don't care
 */
#define EMMC_CMDTM              (EMMC_BASE + 0x0C)

/**
 *  @brief  Response bits 31 : 0
 *
 *  @discussion <#Description#>
 */
#define EMMC_RESP0              (EMMC_BASE + 0x10)

/**
 *  @brief  Response bits 63 : 32
 *
 *  @discussion <#Description#>
 */
#define EMMC_RESP1              (EMMC_BASE + 0x14)

/**
 *  @brief  Response bits 95 : 64
 *
 *  @discussion <#Description#>
 */
#define EMMC_RESP2              (EMMC_BASE + 0x18)

/**
 *  @brief  Response bits 127 : 96
 *
 *  @discussion <#Description#>
 */
#define EMMC_RESP3              (EMMC_BASE + 0x1C)

/**
 *  @brief  Data
 *
 *  @discussion <#Description#>
 */
#define EMMC_DATA               (EMMC_BASE + 0x20)

/**
 *  @brief  Status
 *
 *  @discussion <#Description#>
 */
#define EMMC_STATUS             (EMMC_BASE + 0x24)

/**
 *  @brief  Host Configuration bits
 *
 *  @discussion <#Description#>
 */
#define EMMC_CONTROL0           (EMMC_BASE + 0x28)

/**
 *  @brief  Host Configuration bits
 *
 *  @discussion <#Description#>
 */
#define EMMC_CONTROL1           (EMMC_BASE + 0x2C)

/**
 *  @brief  Interrupt Flags
 *
 *  @discussion <#Description#>
 */
#define EMMC_INTERRUPT          (EMMC_BASE + 0x30)

/**
 *  @brief  Interrupt Flag Enable
 *
 *  @discussion <#Description#>
 */
#define EMMC_IRPT_MASK          (EMMC_BASE + 0x34)

/**
 *  @brief  Interrupt Generation Enable
 *
 *  @discussion <#Description#>
 */
#define EMMC_IRPT_EN            (EMMC_BASE + 0x38)

/**
 *  @brief  Host Configuration bits
 *
 *  @discussion <#Description#>
 */
#define EMMC_CONTROL2           (EMMC_BASE + 0x3C)

/**
 *  @brief  Force Interrupt Event
 *
 *  @discussion <#Description#>
 */
#define EMMC_FORCE_IRPT         (EMMC_BASE + 0x50)

/**
 *  @brief  Timeout in boot mode
 *
 *  @discussion <#Description#>
 */
#define EMMC_BOOT_TIMEOUT       (EMMC_BASE + 0x70)

/**
 *  @brief  Debug Bus Configuration
 *
 *  @discussion <#Description#>
 */
#define EMMC_DBG_SEL            (EMMC_BASE + 0x74)

/**
 *  @brief  Extension FIFO Configuration
 *
 *  @discussion <#Description#>
 */
#define EMMC_EXRDFIFO_CFG       (EMMC_BASE + 0x80)

/**
 *  @brief  Extension FIFO Enable
 *
 *  @discussion <#Description#>
 */
#define EMMC_EXRDFIFO_EN        (EMMC_BASE + 0x84)

/**
 *  @brief  Delay per card clock tuning step
 *
 *  @discussion <#Description#>
 */
#define EMMC_TUNE_STEP          (EMMC_BASE + 0x88)

/**
 *  @brief  Card clock tuning steps for SDR
 *
 *  @discussion <#Description#>
 */
#define EMMC_TUNE_STEPS_STD     (EMMC_BASE + 0x8C)

/**
 *  @brief  Card clock tuning steps for DDR
 *
 *  @discussion <#Description#>
 */
#define EMMC_TUNE_STEPS_DDR     (EMMC_BASE + 0x90)

/**
 *  @brief  SPI Interrupt Support
 *
 *  @discussion <#Description#>
 */
#define EMMC_SPI_INT_SPT        (EMMC_BASE + 0xF0)

/**
 *  @brief  Slot Interrupt Status and Version
 *
 *  @discussion <#Description#>
 */
#define EMMC_SLOTISR_VER        (EMMC_BASE + 0xFC)

namespace raspberryPi {
    namespace EMMC {
        bool reset();
        bool init();
        bool isSDCardInserted();
        uint32_t version();
    }
}

#endif /* __EMMC_H */
