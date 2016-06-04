//
//  EMMC.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/raspberryPi.h>
#include <raspberrypi/EMMC.h>
#include <raspberrypi/GPIO.h>
#include <raspberrypi/Clocks.h>
#include <raspberrypi/Memory.h>

// DEBUG
#include <raspberrypi/Graphics.h>

#ifndef SDHCI_H_
#define SDHCI_H_

#define SDHCI_DIVIDER_SHIFT		8
#define SDHCI_DIVIDER_HI_SHIFT	6
#define SDHCI_DIV_MASK			0xFF
#define SDHCI_DIV_MASK_LEN		8
#define SDHCI_DIV_HI_MASK		0x300

#define SDHCI_MAX_DIV_SPEC_200	256
#define SDHCI_MAX_DIV_SPEC_300	2046

#define  SDHCI_INT_RESPONSE     0x00000001
#define  SDHCI_INT_DATA_END     0x00000002
#define  SDHCI_INT_BLK_GAP      0x00000004
#define  SDHCI_INT_DMA_END      0x00000008
#define  SDHCI_INT_SPACE_AVAIL  0x00000010
#define  SDHCI_INT_DATA_AVAIL   0x00000020
#define  SDHCI_INT_CARD_INSERT  0x00000040
#define  SDHCI_INT_CARD_REMOVE  0x00000080
#define  SDHCI_INT_CARD_INT     0x00000100
#define  SDHCI_INT_ERROR        0x00008000
#define  SDHCI_INT_TIMEOUT      0x00010000
#define  SDHCI_INT_CRC          0x00020000
#define  SDHCI_INT_END_BIT      0x00040000
#define  SDHCI_INT_INDEX        0x00080000
#define  SDHCI_INT_DATA_TIMEOUT 0x00100000
#define  SDHCI_INT_DATA_CRC     0x00200000
#define  SDHCI_INT_DATA_END_BIT 0x00400000
#define  SDHCI_INT_BUS_POWER    0x00800000
#define  SDHCI_INT_ACMD12ERR    0x01000000
#define  SDHCI_INT_ADMA_ERROR   0x02000000

#endif /* SDHCI_H_ */

/**
 *  @brief  SD Clocks
 */
#define SD_CLOCK_ID         	4000000
#define SD_CLOCK_NORMAL     	25000000
#define SD_CLOCK_HIGH       	50000000
#define SD_CLOCK_100        	100000000
#define SD_CLOCK_208        	208000000

#define SD_VER_UNKNOWN      	0
#define SD_VER_1            	1
#define SD_VER_1_1          	2
#define SD_VER_2            	3
#define SD_VER_3            	4
#define SD_VER_4            	5

#define SD_CMD_INDEX(a)				(uint32_t)((a) << 24)
#define SD_CMD_TYPE_NORMAL			0x0
#define SD_CMD_TYPE_SUSPEND			(1 << 22)
#define SD_CMD_TYPE_RESUME			(2 << 22)
#define SD_CMD_TYPE_ABORT			(3 << 22)
#define SD_CMD_TYPE_MASK			(3 << 22)
#define SD_CMD_ISDATA				(1 << 21)
#define SD_CMD_IXCHK_EN				(1 << 20)
#define SD_CMD_CRCCHK_EN			(1 << 19)
#define SD_CMD_RSPNS_TYPE_NONE		0			// For no response
#define SD_CMD_RSPNS_TYPE_136		(1 << 16)	// For response R2 (with CRC), R3,4 (no CRC)
#define SD_CMD_RSPNS_TYPE_48		(2 << 16)	// For responses R1, R5, R6, R7 (with CRC)
#define SD_CMD_RSPNS_TYPE_48B		(3 << 16)	// For responses R1b, R5b (with CRC)
#define SD_CMD_RSPNS_TYPE_MASK 		(3 << 16)
#define SD_CMD_MULTI_BLOCK			(1 << 5)
#define SD_CMD_DAT_DIR_HC			0
#define SD_CMD_DAT_DIR_CH			(1 << 4)
#define SD_CMD_AUTO_CMD_EN_NONE		0
#define SD_CMD_AUTO_CMD_EN_CMD12	(1 << 2)
#define SD_CMD_AUTO_CMD_EN_CMD23	(2 << 2)
#define SD_CMD_BLKCNT_EN			(1 << 1)
#define SD_CMD_DMA          		1

#define SD_ERR_CMD_TIMEOUT		0
#define SD_ERR_CMD_CRC			1
#define SD_ERR_CMD_END_BIT		2
#define SD_ERR_CMD_INDEX		3
#define SD_ERR_DATA_TIMEOUT		4
#define SD_ERR_DATA_CRC			5
#define SD_ERR_DATA_END_BIT		6
#define SD_ERR_CURRENT_LIMIT	7
#define SD_ERR_AUTO_CMD12		8
#define SD_ERR_ADMA				9
#define SD_ERR_TUNING			10
#define SD_ERR_RSVD				11

#define SD_ERR_MASK_CMD_TIMEOUT		(1 << (16 + SD_ERR_CMD_TIMEOUT))
#define SD_ERR_MASK_CMD_CRC			(1 << (16 + SD_ERR_CMD_CRC))
#define SD_ERR_MASK_CMD_END_BIT		(1 << (16 + SD_ERR_CMD_END_BIT))
#define SD_ERR_MASK_CMD_INDEX		(1 << (16 + SD_ERR_CMD_INDEX))
#define SD_ERR_MASK_DATA_TIMEOUT	(1 << (16 + SD_ERR_CMD_TIMEOUT))
#define SD_ERR_MASK_DATA_CRC		(1 << (16 + SD_ERR_CMD_CRC))
#define SD_ERR_MASK_DATA_END_BIT	(1 << (16 + SD_ERR_CMD_END_BIT))
#define SD_ERR_MASK_CURRENT_LIMIT	(1 << (16 + SD_ERR_CMD_CURRENT_LIMIT))
#define SD_ERR_MASK_AUTO_CMD12		(1 << (16 + SD_ERR_CMD_AUTO_CMD12))
#define SD_ERR_MASK_ADMA			(1 << (16 + SD_ERR_CMD_ADMA))
#define SD_ERR_MASK_TUNING			(1 << (16 + SD_ERR_CMD_TUNING))

#define SD_RESP_NONE        SD_CMD_RSPNS_TYPE_NONE
#define SD_RESP_R1          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R1b         (SD_CMD_RSPNS_TYPE_48B | SD_CMD_CRCCHK_EN)
#define SD_RESP_R2          (SD_CMD_RSPNS_TYPE_136 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R3          SD_CMD_RSPNS_TYPE_48
#define SD_RESP_R4          SD_CMD_RSPNS_TYPE_136
#define SD_RESP_R5          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R5b         (SD_CMD_RSPNS_TYPE_48B | SD_CMD_CRCCHK_EN)
#define SD_RESP_R6          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R7          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)

#define SD_DATA_READ        (SD_CMD_ISDATA | SD_CMD_DAT_DIR_CH)
#define SD_DATA_WRITE       (SD_CMD_ISDATA | SD_CMD_DAT_DIR_HC)

#define SD_CMD_RESERVED(a)  (uint32_t)0xffffffff

/**
 *  @brief  SD Interrupt masks
 */
#define SD_COMMAND_COMPLETE		(1 << 0)
#define SD_TRANSFER_COMPLETE	(1 << 1)
#define SD_BLOCK_GAP_EVENT		(1 << 2)
#define SD_DMA_INTERRUPT		(1 << 3)
#define SD_BUFFER_WRITE_READY	(1 << 4)
#define SD_BUFFER_READ_READY	(1 << 5)
#define SD_CARD_INSERTION		(1 << 6)
#define SD_CARD_REMOVAL			(1 << 7)
#define SD_CARD_INTERRUPT		(1 << 8)

#define SUCCESS(a)          	(a->last_cmd_success)
#define FAIL(a)            	 	(a->last_cmd_success == 0)
#define TIMEOUT(a)         		(FAIL(a) && (a->last_error == 0))
#define CMD_TIMEOUT(a)     		(FAIL(a) && (a->last_error & (1 << 16)))
#define CMD_CRC(a)        	 	(FAIL(a) && (a->last_error & (1 << 17)))
#define CMD_END_BIT(a)     		(FAIL(a) && (a->last_error & (1 << 18)))
#define CMD_INDEX(a)       		(FAIL(a) && (a->last_error & (1 << 19)))
#define DATA_TIMEOUT(a)  		(FAIL(a) && (a->last_error & (1 << 20)))
#define DATA_CRC(a)				(FAIL(a) && (a->last_error & (1 << 21)))
#define DATA_END_BIT(a)			(FAIL(a) && (a->last_error & (1 << 22)))
#define CURRENT_LIMIT(a)		(FAIL(a) && (a->last_error & (1 << 23)))
#define ACMD12_ERROR(a)			(FAIL(a) && (a->last_error & (1 << 24)))
#define ADMA_ERROR(a)			(FAIL(a) && (a->last_error & (1 << 25)))
#define TUNING_ERROR(a)			(FAIL(a) && (a->last_error & (1 << 26)))

// The actual command indices
#define GO_IDLE_STATE           0
#define ALL_SEND_CID            2
#define SEND_RELATIVE_ADDR      3
#define SET_DSR                 4
#define IO_SET_OP_COND          5
#define SWITCH_FUNC             6
#define SELECT_CARD             7
#define DESELECT_CARD           7
#define SELECT_DESELECT_CARD    7
#define SEND_IF_COND            8
#define SEND_CSD                9
#define SEND_CID                10
#define VOLTAGE_SWITCH          11
#define STOP_TRANSMISSION       12
#define SEND_STATUS             13
#define GO_INACTIVE_STATE       15
#define SET_BLOCKLEN            16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK     18
#define SEND_TUNING_BLOCK       19
#define SPEED_CLASS_CONTROL     20
#define SET_BLOCK_COUNT         23
#define WRITE_BLOCK             24
#define WRITE_MULTIPLE_BLOCK    25
#define PROGRAM_CSD             27
#define SET_WRITE_PROT          28
#define CLR_WRITE_PROT          29
#define SEND_WRITE_PROT         30
#define ERASE_WR_BLK_START      32
#define ERASE_WR_BLK_END        33
#define ERASE                   38
#define LOCK_UNLOCK             42
#define APP_CMD                 55
#define GEN_CMD                 56

#define IS_APP_CMD              0x80000000
#define ACMD(a)                 (a | IS_APP_CMD)
#define SET_BUS_WIDTH           (6 | IS_APP_CMD)
#define SD_STATUS               (13 | IS_APP_CMD)
#define SEND_NUM_WR_BLOCKS      (22 | IS_APP_CMD)
#define SET_WR_BLK_ERASE_COUNT  (23 | IS_APP_CMD)
#define SD_SEND_OP_COND         (41 | IS_APP_CMD)
#define SET_CLR_CARD_DETECT     (42 | IS_APP_CMD)
#define SEND_SCR                (51 | IS_APP_CMD)

// BLKSIZECNT, offset 0x04
#define EMMC_BLKSIZECNT_BLKSIZE_MASK	((uint32_t)0x000003ff)	///< SDHCI_BLOCK_SIZE    0x04
#define EMMC_BLKSIZECNT_BLKCNT_MASK		((uint32_t)0xffff0000)	///< SDHCI_BLOCK_COUNT   0x06

// STATUS, offset 0x24
#define EMMC_STATUS_CMD_INHIBIT			((uint32_t)0x00000001)	///< SDHCI_PRESENT_STATE 0x24, SDHCI_CMD_INHIBIT 0x00000001
#define EMMC_STATUS_DATA_INHIBIT		((uint32_t)0x00000002)	///< SDHCI_PRESENT_STATE 0x24, SDHCI_DATA_INHIBIT 0x00000002
#define EMMC_STATUS_CARD_PRESENT   		((uint32_t)0x00010000)	///< SDHCI_PRESENT_STATE 0x24, SDHCI_CARD_PRESENT 0x00010000

// CONTROL1, offset 0x2C
#define EMMC_CONTROL1_CLK_INTLEN		((uint32_t)(1 << 0))	///< SDHCI_CLOCK_CONTROL  0x2C, SDHCI_CLOCK_INT_EN 0x0001
#define EMMC_CONTROL1_CLK_STABLE		((uint32_t)(1 << 1))	///< SDHCI_CLOCK_CONTROL  0x2C, SDHCI_CLOCK_INT_STABLE 0x0002
#define EMMC_CONTROL1_CLK_EN			((uint32_t)(1 << 2))	///< SDHCI_CLOCK_CONTROL  0x2C, SDHCI_CLOCK_CARD_EN	0x0004
// SDHCI_TIMEOUT_CONTROL 0x2E
#define EMMC_CONTROL1_SRST_HC 			((uint32_t)(1 << 24))	///< SDHCI_SOFTWARE_RESET 0x2F, SDHCI_RESET_ALL 0x01
#define EMMC_CONTROL1_RESET_CMD 		((uint32_t)(1 << 25))	///< SDHCI_SOFTWARE_RESET 0x2F, SDHCI_RESET_CMD 0x02
#define EMMC_CONTROL1_RESET_DATA		((uint32_t)(1 << 26))	///< SDHCI_SOFTWARE_RESET 0x2F, SDHCI_RESET_DATA 0x04

// SLOTISR_VER, offset 0xFC --> SDHCI_SLOT_INT_STATUS 0xFC
//                          --> SDHCI_HOST_VERSION    0xFE
#define EMMC_SLOTISR_VER_SLOT_STATUS_MASK	((uint32_t)(0x000000FF))///< SDHCI_SLOT_INT_STATUS 0xFC
#define EMMC_SLOTISR_VER_VENDOR_MASK		((uint32_t)(0xFF000000))///< SDHCI_HOST_VERSION 0xFE, SDHCI_VENDOR_VER_MASK 0xFF00
#define EMMC_SLOTISR_VER_VENDOR_SHIFT		24						///< 	SDHCI_VENDOR_VER_SHIFT 8
#define EMMC_SLOTISR_VER_SDVERSION_MASK		((uint32_t)(0x00FF0000))///< SDHCI_HOST_VERSION 0xFE, SDHCI_SPEC_VER_MASK 0x00FF
#define EMMC_SLOTISR_VER_SDVERSION_SHIFT	16						///< 	SDHCI_SPEC_VER_SHIFT   0

#define SD_GET_CLOCK_DIVIDER_FAIL	0xffffffff

#define MIN_FREQ 400000
#define EMMC_WRITE_DELAY       (((2 * 1000000) / MIN_FREQ) + 1)

using namespace raspberryPi;
using namespace Graphics;

const uint32_t sd_commands[] __attribute__((aligned(4))) = {
    SD_CMD_INDEX(0),
    SD_CMD_RESERVED(1),
    SD_CMD_INDEX(2) | SD_RESP_R2,
    SD_CMD_INDEX(3) | SD_RESP_R6,
    SD_CMD_INDEX(4),
    SD_CMD_INDEX(5) | SD_RESP_R4,
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_INDEX(7) | SD_RESP_R1b,
    SD_CMD_INDEX(8) | SD_RESP_R7,
    SD_CMD_INDEX(9) | SD_RESP_R2,
    SD_CMD_INDEX(10) | SD_RESP_R2,
    SD_CMD_INDEX(11) | SD_RESP_R1,
    SD_CMD_INDEX(12) | SD_RESP_R1b | SD_CMD_TYPE_ABORT,
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_INDEX(15),
    SD_CMD_INDEX(16) | SD_RESP_R1,
    SD_CMD_INDEX(17) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(18) | SD_RESP_R1 | SD_DATA_READ | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_INDEX(19) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(20) | SD_RESP_R1b,
    SD_CMD_RESERVED(21),
    SD_CMD_RESERVED(22),
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_INDEX(24) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(25) | SD_RESP_R1 | SD_DATA_WRITE | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_RESERVED(26),
    SD_CMD_INDEX(27) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(28) | SD_RESP_R1b,
    SD_CMD_INDEX(29) | SD_RESP_R1b,
    SD_CMD_INDEX(30) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(31),
    SD_CMD_INDEX(32) | SD_RESP_R1,
    SD_CMD_INDEX(33) | SD_RESP_R1,
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_INDEX(38) | SD_RESP_R1b,
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_RESERVED(41),
    SD_CMD_RESERVED(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_RESERVED(51),
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_INDEX(55) | SD_RESP_R1,
    SD_CMD_INDEX(56) | SD_RESP_R1 | SD_CMD_ISDATA,
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

static const uint32_t sd_acommands[] __attribute__((aligned(4))) = {
    SD_CMD_RESERVED(0),
    SD_CMD_RESERVED(1),
    SD_CMD_RESERVED(2),
    SD_CMD_RESERVED(3),
    SD_CMD_RESERVED(4),
    SD_CMD_RESERVED(5),
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_RESERVED(7),
    SD_CMD_RESERVED(8),
    SD_CMD_RESERVED(9),
    SD_CMD_RESERVED(10),
    SD_CMD_RESERVED(11),
    SD_CMD_RESERVED(12),
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_RESERVED(15),
    SD_CMD_RESERVED(16),
    SD_CMD_RESERVED(17),
    SD_CMD_RESERVED(18),
    SD_CMD_RESERVED(19),
    SD_CMD_RESERVED(20),
    SD_CMD_RESERVED(21),
    SD_CMD_INDEX(22) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_RESERVED(24),
    SD_CMD_RESERVED(25),
    SD_CMD_RESERVED(26),
    SD_CMD_RESERVED(27),
    SD_CMD_RESERVED(28),
    SD_CMD_RESERVED(29),
    SD_CMD_RESERVED(30),
    SD_CMD_RESERVED(31),
    SD_CMD_RESERVED(32),
    SD_CMD_RESERVED(33),
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_RESERVED(38),
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_INDEX(41) | SD_RESP_R3,
    SD_CMD_INDEX(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_INDEX(51) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_RESERVED(55),
    SD_CMD_RESERVED(56),
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

#define TIMEOUT_WAIT(stop_if_true, usec) 						\
do {															\
    uint32_t micros_now = GET32(0xF2003004);					\
    do {														\
        if(stop_if_true)										\
            break;												\
    } while( GET32(0xF2003004) - micros_now < (uint32_t)usec);	\
} while(0);

struct sd_scr {
    uint32_t scr[2];
    uint32_t sd_bus_widths;
    int sd_version;
};

struct block_device {
    uint64_t block_size;
};

struct emmc_block_dev {
    struct block_device bd;
    uint32_t card_supports_sdhc;
    uint32_t card_supports_18v;
    uint32_t card_ocr;
    uint32_t card_rca;
    uint32_t last_interrupt;
    uint32_t last_error;
    
    struct sd_scr *scr;
    
    int failed_voltage_switch;
    
    uint32_t last_cmd_reg;
    uint32_t last_cmd;
    uint32_t last_cmd_success;
    uint32_t last_r0;
    uint32_t last_r1;
    uint32_t last_r2;
    uint32_t last_r3;
    
    void *buf;
    int blocks_to_transfer;
    uint64_t block_size;
    int card_removal;
};

static struct sd_scr sdcard_scr  __attribute__((aligned(4)));
static struct emmc_block_dev block_dev __attribute__((aligned(4)));

namespace raspberryPi {
    namespace EMMC {
        uint32_t clockDivider(uint32_t clock);
        bool setClock(uint32_t clock);
        void sendCommandSD(uint32_t command, uint32_t argument, uint32_t timeout);
        void sendCommandEMMC(uint32_t cmdRegister, uint32_t argument, uint32_t timeout);
        void handleInterrupts();
        void handleCardInterrupt();
        bool resetCMD();
        void setBlockSize(uint32_t size);
    }
}

uint32_t EMMC::clockDivider(uint32_t clock) {
    uint32_t targetted_divisor = 0;
    uint32_t base = Clocks::rate(Clocks::EMMC);
    
    if (clock > base) {
        targetted_divisor = 1;
    } else {
        targetted_divisor = div(base, clock);
        uint32_t mods = mod(base, clock);
        if (mods) {
            targetted_divisor++;
        }
    }
    
    int divisor = -1;
    for (int first_bit = 31; first_bit >= 0; first_bit--) {
        uint32_t bit_test = (1 << first_bit);
        if (targetted_divisor & bit_test) {
            divisor = first_bit;
            targetted_divisor &= ~bit_test;
            if (targetted_divisor) {
                divisor++;
            }
            break;
        }
    }
    
    if(divisor == -1) {
        divisor = 31;
    }
    
    if(divisor >= 32) {
        divisor = 31;
    }
    
    if(divisor != 0) {
        divisor = (1 << (divisor - 1));
    }
    
    if(divisor >= 0x400) {
        divisor = 0x3ff;
    }
    
    uint32_t ret = (divisor & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
    ret |= ((divisor & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN) << SDHCI_DIVIDER_HI_SHIFT;
    
    return ret;
}

bool EMMC::setClock(uint32_t clock) {
    const uint32_t divider = clockDivider(clock);
    
    if (divider == SD_GET_CLOCK_DIVIDER_FAIL) {
        Console::log("Couldn't get a valid divider for target clock\n");
        return false;
    }
    
    TIMEOUT_WAIT(!(GET32(EMMC_STATUS) & (EMMC_STATUS_CMD_INHIBIT | EMMC_STATUS_DATA_INHIBIT)), 1000000);
    
    if ((GET32(EMMC_STATUS) & (EMMC_STATUS_CMD_INHIBIT | EMMC_STATUS_DATA_INHIBIT)) != 0) {
        Console::log("Timeout waiting for inhibit flags.\n");
        return false;
    }
    
    // Set the SD clock off
    uint32_t control1 = GET32(EMMC_CONTROL1);
    control1 &= ~ EMMC_CONTROL1_CLK_EN;
    PUT32(EMMC_CONTROL1, control1);
    waits(EMMC_WRITE_DELAY);
    
    // Write the new divider
    control1 &= ~ 0xFFE0;		// Clear old setting + clock generator select
    control1 |= divider;
    PUT32(EMMC_CONTROL1, control1);
    waits(EMMC_WRITE_DELAY);
    
    // Enable the SD clock
    control1 |= EMMC_CONTROL1_CLK_EN;
    PUT32(EMMC_CONTROL1, control1);
    waits(EMMC_WRITE_DELAY);
    
    TIMEOUT_WAIT(GET32(EMMC_CONTROL1) & EMMC_CONTROL1_CLK_STABLE, 1000000);
    
    if ((GET32(EMMC_CONTROL1) & EMMC_CONTROL1_CLK_STABLE) == 0) {
        Graphics::Console::log("Controller's clock did not stabilize within 1 second\n");
        return -1;
    }
    
    Graphics::Console::log("Successfully set clock rate\n");
    return true;
}

void EMMC::sendCommandSD(uint32_t command, uint32_t argument, uint32_t timeout) {
    Console::nls(Console::INFO, "sendCommandSD ");
    Graphics::Console::logHexString(command);
    Graphics::Console::logHexString(argument);
    Graphics::Console::logHexStringLine(timeout);
    struct emmc_block_dev * dev = &block_dev;
    
    handleInterrupts();
    
    if (dev->card_removal) {
        dev->last_cmd_success = 0;
        return;
    }
    
    if (command & IS_APP_CMD) {
        command &= 0xFF;
        
        Console::nls(Console::INFO, "Issuing command ACMD ");
        Graphics::Console::logHexStringLine(command);
        
        if (sd_commands[command] == SD_CMD_RESERVED(0)) {
            Console::nls(Console::FAILED, "Invalid command ACMD ");
            Graphics::Console::logHexStringLine(command);
            dev->last_cmd_success = 0;
            return;
        }
        
        dev->last_cmd = APP_CMD;
        
        uint32_t rca = 0;
        if (dev->card_rca) {
            rca = dev->card_rca << 16;
        }
        
        sendCommandEMMC(sd_commands[APP_CMD], rca, timeout);
        
        if (dev->last_cmd_success) {
            dev->last_cmd = command | IS_APP_CMD;
            sendCommandEMMC(sd_acommands[command], argument, timeout);
        }
    } else {
        Console::nls(Console::INFO, "Issuing command CMD ");
        Graphics::Console::logHexStringLine(command);
        if (sd_commands[command] == SD_CMD_RESERVED(0)) {
            Console::nls(Console::FAILED, "Invalid command CMD ");
            Graphics::Console::logHexStringLine(command);
            dev->last_cmd_success = 0;
            return;
        }
        
        dev->last_cmd = command;
        sendCommandEMMC(sd_commands[command], argument,	timeout);
    }
}

void EMMC::sendCommandEMMC(uint32_t cmdRegister, uint32_t argument, uint32_t timeout) {
    Console::nls(Console::INFO, "sendCommandEMMC ");
    Graphics::Console::logHexString(cmdRegister);
    Graphics::Console::logHexString(argument);
    Graphics::Console::logHexStringLine(timeout);
    struct emmc_block_dev * dev = &block_dev;
    
    dev->last_cmd_reg = cmdRegister;
    dev->last_cmd_success = 0;
    
    // 等待命令执行
    while (GET32(EMMC_STATUS) & EMMC_STATUS_CMD_INHIBIT);
    
    // 检查是否为命令正忙
    if ((cmdRegister & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) {
        // 是否为ABORT命令
        if ((cmdRegister & SD_CMD_TYPE_MASK) != SD_CMD_TYPE_ABORT) {
            // 不是ABORT命令
            // 那就等它执行完
            while (GET32(EMMC_STATUS) & EMMC_STATUS_DATA_INHIBIT);
        }
    }
    
    // 设置块大小和块总数
    if (dev->blocks_to_transfer > 0xFFFF) {
        Console::nls(Console::FAILED, "Blocks_to_transfer too great ");
        Graphics::Console::logHexStringLine(dev->blocks_to_transfer);
        dev->last_cmd_success = 0;
        return;
    }
    
    uint32_t blockSizeCount = (uint32_t)dev->block_size | (dev->blocks_to_transfer << 16);
    PUT32(EMMC_BLKSIZECNT, blockSizeCount);
    
    PUT32(EMMC_ARG1, argument);
    
    PUT32(EMMC_CMDTM, cmdRegister);
    waits(EMMC_WRITE_DELAY);
    
    Console::nls(Console::INFO, "Wait for command complete interrupt\n");
    
    uint32_t irqs;
    uint32_t micros_now = clock();
    do {
        if((irqs = GET32(EMMC_INTERRUPT) & 0x8001))
            break;
    } while(clock() - micros_now < 5000000);
    
    PUT32(EMMC_INTERRUPT, 0xFFFF0001);
    
    if ((irqs & 0xFFFF0001) != 0x1) {
        Console::nls(Console::FAILED, "Error occurred whilst waiting for command complete interrupt\n");
        dev->last_error = irqs & 0xFFFF0000;
        Console::nls(Console::FAILED, "bit 15 ERR ");
        Graphics::Console::logHexStringLine(dev->last_error);
        dev->last_interrupt = irqs;
        return;
    }
    
    switch (cmdRegister & SD_CMD_RSPNS_TYPE_MASK) {
        case SD_CMD_RSPNS_TYPE_48:
        case SD_CMD_RSPNS_TYPE_48B:
            dev->last_r0 = GET32(EMMC_RESP0);
            break;
        case SD_CMD_RSPNS_TYPE_136:
            dev->last_r0 = GET32(EMMC_RESP0);
            dev->last_r1 = GET32(EMMC_RESP1);
            dev->last_r2 = GET32(EMMC_RESP2);
            dev->last_r3 = GET32(EMMC_RESP3);
            break;
    }
    
    if (cmdRegister & SD_CMD_ISDATA) {
        uint32_t rwIrq;
        bool write = 0;
        
        if (cmdRegister & SD_CMD_DAT_DIR_CH) {
            rwIrq = (1 << 5);
        } else {
            rwIrq = (1 << 4);
            write = true;
        }
        
        int currentBlock = 0;
        uint32_t * currentBufferAddress = (uint32_t *)dev->buf;
        while (currentBlock < dev->blocks_to_transfer) {
            TIMEOUT_WAIT(GET32(EMMC_INTERRUPT) & (rwIrq | 0x8000), timeout);
            irqs = GET32(EMMC_INTERRUPT);
            PUT32(EMMC_INTERRUPT, (0xFFFF0000 | rwIrq));
            
            if ((irqs & (0xFFFF0000 | rwIrq)) != rwIrq) {
                Console::nls(Console::FAILED, "Error occurred whilst waiting for data ready interrupt");
                dev->last_error = irqs & 0xFFFF0000;
                dev->last_interrupt = irqs;
                return;
            }
            
            // 传输块
            uint64_t currentByteNumber = 0;
            while (currentByteNumber < dev->block_size) {
                if (write) {
                    PUT32(EMMC_DATA, *currentBufferAddress);
                } else {
                    *currentBufferAddress = GET32(EMMC_DATA);
                }
                currentByteNumber += 4;
                currentBufferAddress++;
            }
            Console::nls(Console::INFO, "block ");
            Graphics::Console::logHexString(currentBlock);
            Graphics::Console::log("transfer complete\n");
            currentBlock++;
        }
    }
    
    // 等待传输完成
    if((((cmdRegister & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) || (cmdRegister & SD_CMD_ISDATA))) {
        // 检查高位是否已经不是0了
        if ((GET32(EMMC_STATUS) & EMMC_STATUS_DATA_INHIBIT) == 0) {
            PUT32(EMMC_INTERRUPT, 0xFFFF0002);
        } else {
            TIMEOUT_WAIT(GET32(EMMC_INTERRUPT) & 0x8002, timeout);
            irqs = GET32(EMMC_INTERRUPT);
            PUT32(EMMC_INTERRUPT, 0xFFFF0002);
            
            if (((irqs & 0xFFFF0002) != 0x2) && ((irqs & 0xFFFF0002) != 0x100002)) {
                Console::nls(Console::FAILED, "Error occurred whilst waiting for transfer complete interrupt\n");
                dev->last_error = irqs & 0xFFFF0000;
                dev->last_interrupt = irqs;
                return;
            }
            
            PUT32(EMMC_INTERRUPT, 0xFFFF0002);
        }
    }
    
    dev->last_cmd_success = 1;
}

void EMMC::handleInterrupts() {
    struct emmc_block_dev * dev = &block_dev;
    
    const uint32_t irqs = GET32(EMMC_INTERRUPT);
    uint32_t resetMask = 0;
    
    if (irqs & SD_COMMAND_COMPLETE) {
        resetMask |= SD_COMMAND_COMPLETE;
    }
    
    if (irqs & SD_TRANSFER_COMPLETE) {
        resetMask |= SD_TRANSFER_COMPLETE;
    }
    
    if (irqs & SD_BLOCK_GAP_EVENT) {
        resetMask |= SD_BLOCK_GAP_EVENT;
    }
    
    if (irqs & SD_DMA_INTERRUPT) {
        resetMask |= SD_DMA_INTERRUPT;
    }
    
    if (irqs & SD_BUFFER_WRITE_READY) {
        resetMask |= SD_BUFFER_WRITE_READY;
    }
    
    if (irqs & SD_BUFFER_READ_READY) {
        resetMask |= SD_BUFFER_READ_READY;
    }
    
    if (irqs & SD_CARD_INSERTION) {
        resetMask |= SD_CARD_INSERTION;
    }
    
    if (irqs & SD_CARD_REMOVAL) {
        resetMask |= SD_CARD_REMOVAL;
        dev->card_removal = 1;
    }
    
    if (irqs & SD_CARD_INTERRUPT) {
        resetMask |= SD_CARD_INTERRUPT;
    }
    
    if (irqs & 0x8000) {
        resetMask |= 0xFFFF0000;
    }
    
    PUT32(EMMC_INTERRUPT, resetMask);
}

void EMMC::handleCardInterrupt() {
    struct emmc_block_dev * dev = &block_dev;
    
    Console::nls(Console::INFO, "Controller status: ");
    Graphics::Console::logHexStringLine(GET32(EMMC_STATUS));
    Console::nls(Console::INFO, "Get the card status\n");
    
    if (dev->card_rca) {
        sendCommandEMMC(sd_commands[SEND_STATUS], dev->card_rca << 16, 500000);
        
        if (FAIL(dev)) {
            Console::nls(Console::FAILED, "Unable to get card status\n");
        } else {
            Console::nls(Console::INFO, "Card status: ");
            Graphics::Console::logHexStringLine(dev->last_r0);
        }
    } else {
        Console::nls(Console::INFO, "No card currently selected\n");
    }
}

bool EMMC::resetCMD() {
    uint32_t control1 = GET32(EMMC_CONTROL1);
    control1 |= EMMC_CONTROL1_RESET_CMD;
    PUT32(EMMC_CONTROL1, control1);
    waits(EMMC_WRITE_DELAY);
    
    TIMEOUT_WAIT((GET32(EMMC_CONTROL1) & EMMC_CONTROL1_RESET_CMD) == 0, 1000000);
    
    if ((GET32(EMMC_CONTROL1) & EMMC_CONTROL1_RESET_CMD) != 0) {
        return false;
    }
    
    PUT32(EMMC_INTERRUPT, SD_ERR_MASK_CMD_TIMEOUT);
    waits(EMMC_WRITE_DELAY);
    
    return true;
}

void EMMC::setBlockSize(uint32_t size) {
    uint32_t blockSize = GET32(EMMC_BLKSIZECNT);
    blockSize &= ~EMMC_BLKSIZECNT_BLKSIZE_MASK;
    blockSize |= size;
    PUT32(EMMC_BLKSIZECNT, blockSize);
    waits(EMMC_WRITE_DELAY);
}

bool EMMC::reset() {
    PUT32(EMMC_CONTROL0, 0);
    PUT32(EMMC_CONTROL2, 0);
    
    uint32_t control1 = GET32(EMMC_CONTROL1);
    control1 |= EMMC_CONTROL1_SRST_HC;
    PUT32(EMMC_CONTROL1, control1);
    
    waits(EMMC_WRITE_DELAY);
    TIMEOUT_WAIT((GET32(EMMC_CONTROL1) & EMMC_CONTROL1_SRST_HC) == 0, 1000000);
    
    if ((GET32(EMMC_CONTROL1) & EMMC_CONTROL1_SRST_HC) != 0) {
        return false;
    }
    
    control1 = GET32(EMMC_CONTROL1);
    control1 |= (1 << 0);
    control1 |= 0x000E0000;
    PUT32(EMMC_CONTROL1, control1);
    waits(EMMC_WRITE_DELAY);
    
    if (!Clocks::setRate(raspberryPi::Clocks::EMMC, SD_CLOCK_ID)) {
        return false;
    }
    
    PUT32(EMMC_IRPT_EN, 0);
    PUT32(EMMC_INTERRUPT, 0xFFFFFFFF);
    PUT32(EMMC_IRPT_MASK, 0xFFFFFFFF & (~ SD_CARD_INTERRUPT));
    waits(EMMC_WRITE_DELAY);
    
    return true;
}

bool EMMC::init() {
    if (!isSDCardInserted()) return false;
    
    if (version() < 2) {
        Console::nls(Console::FAILED, "Only SDHCI versions >= 3.0 are supported\n");
        return false;
    } else {
        Console::nls(Console::OK, "SDHCI versions >= 3.0\n");
    }
    
    GPIO::setFunction(GPIO_CD, GPIO::INPUT);
    GPIO::pull(GPIO_CD, GPIO::PULL_UP);
    GPIO::detectEvent(GPIO_CD, GPIO::HIGH);
    
    if (!reset()) {
        Console::nls(Console::FAILED, "Controller did not reset properly\n");
        return false;
    }
    
    struct emmc_block_dev *ret = &block_dev;
    Memory::memclr(ret, sizeof(struct emmc_block_dev));
    ret->bd.block_size = 512;
    
//    // Send CMD0 to the card (reset to idle state)
//    sendCommandSD(GO_IDLE_STATE, 0, 500000);
//    if (FAIL(ret)) {
//        Graphics::Console::log("No CMD0 response\n");
//        return false;
//    }
    
    // Send CMD8 to the card
    // Voltage supplied = 0x1 = 2.7-3.6V (standard)
    // Check pattern = 10101010b (as per PLSS 4.3.13) = 0xAA
    Console::nls(Console::INFO, "Note a timeout error on the following command (CMD8) is normal and expected if the SD card version is less than 2.0\n");
    sendCommandSD(SEND_IF_COND, 0x000001AA, 500000);
    
    int v2_later = 0;
    
    if (TIMEOUT(ret)) {
        v2_later = 0;
    } else if (CMD_TIMEOUT(ret)) {
        if (!resetCMD()) {
            return false;
        }
        v2_later = 0;
    } else if (FAIL(ret)) {
        return false;
    } else {
        if ((ret->last_r0 & 0xFFF) != 0x1AA) {
            return false;
        } else {
            v2_later = 1;
        }
    }
    
    
    // Here we are supposed to check the response to CMD5 (HCSS 3.6) page 100
    // It only returns if the card is a SDIO card
    sendCommandSD(IO_SET_OP_COND, 0, 10000);
    
    if (!TIMEOUT(ret)) {
        if (CMD_TIMEOUT(ret)) {
            if (!resetCMD()) {
                return false;
            }
        } else {
            return false;
        }
    }
    
    // Call an inquiry ACMD41 (voltage window = 0) to get the OCR
    sendCommandSD(ACMD(41), 0, 500000);
    
    if (FAIL(ret)) {
        return false;
    }
    
    // "Inquiry ACMD41 returned %08x", ret->last_r0
    // Call initialization ACMD41
    int card_is_busy = 1;
    while (card_is_busy) {
        uint32_t v2_flags = 0;
        if (v2_later) {
            // Set SDHC support
            v2_flags |= (1 << 30);
            // Enable SDXC maximum performance
            v2_flags |= (1 << 28);
        }
        
        sendCommandSD(ACMD(41), 0x00ff8000 | v2_flags, 500000);
        
        if (FAIL(ret)) {
            return false;
        }
        
        if ((ret->last_r0 >> 31) & 0x1) {
            // Initialization is complete
            ret->card_ocr = (ret->last_r0 >> 8) & 0xffff;
            ret->card_supports_sdhc = (ret->last_r0 >> 30) & 0x1;
            card_is_busy = 0;
        } else {
            waits(5000000);
        }
    }
    
    // At this point, we know the card is definitely an SD card, so will definitely
    //  support SDR12 mode which runs at 25 MHz
    setClock(SD_CLOCK_NORMAL);
    
    // SD_TRACE("Send CMD2 to get the cards CID");
    sendCommandSD(ALL_SEND_CID, 0, 500000);
    
    if (FAIL(ret)) {
        // SD_TRACE("Error sending ALL_SEND_CID");
        return false;
    }
    
    // SD_TRACE("Send CMD3 to enter the data state");
    sendCommandSD(SEND_RELATIVE_ADDR, 0, 500000);
    
    if (FAIL(ret)) {
        // SD_TRACE("Error sending SEND_RELATIVE_ADDR");
        return false;
    }
    
    uint32_t cmd3_resp = ret->last_r0;
    // SD_TRACE("CMD3 response: %08x", cmd3_resp);
    
    ret->card_rca = (cmd3_resp >> 16) & 0xffff;
    uint32_t crc_error = (cmd3_resp >> 15) & 0x1;
    uint32_t illegal_cmd = (cmd3_resp >> 14) & 0x1;
    uint32_t error = (cmd3_resp >> 13) & 0x1;
    uint32_t status = (cmd3_resp >> 9) & 0xf;
    uint32_t ready = (cmd3_resp >> 8) & 0x1;
    
    if (crc_error) {
        // SD_TRACE("CRC error");
        return false;
    }
    
    if (illegal_cmd) {
        // SD_TRACE("Illegal command");
        return false;
    }
    
    if (error) {
        // SD_TRACE("Generic error");
        return false;
    }
    
    if (!ready) {
        // SD_TRACE("Not ready for data");
        return false;
    }
    
    // SD_TRACE("RCA: %04x", ret->card_rca);
    
    // Now select the card (toggles it to transfer state)
    sendCommandSD(SELECT_CARD, ret->card_rca << 16, 500000);
    
    if (FAIL(ret)) {
        // SD_TRACE("Error sending CMD7");
        return false;
    }
    
    uint32_t cmd7_resp = ret->last_r0;
    status = (cmd7_resp >> 9) & 0xf;
    
    if ((status != 3) && (status != 4)) {
        // SD_TRACE("Invalid status (%d)", status);
        return false;
    }
    
    // If not an SDHC card, ensure BLOCKLEN is 512 bytes
    if (!ret->card_supports_sdhc) {
        sendCommandSD(SET_BLOCKLEN, 512, 500000);
        if (FAIL(ret)) {
            // SD_TRACE("Error sending SET_BLOCKLEN");
            return false;
        }
    }
    
    ret->block_size = 512;
    
    setBlockSize((uint32_t)ret->block_size);
    
    // Get the cards SCR register
    // SEND_SCR command is like a READ_SINGLE but for a block of 8 bytes.
    // Ensure that any data operation has completed before reading the block.
    ret->scr = &sdcard_scr;
    ret->buf = &ret->scr->scr[0];
    ret->block_size = 8;
    ret->blocks_to_transfer = 1;
    
    sendCommandSD(SEND_SCR, 0, 500000);
    
    ret->block_size = 512;
    
    if (FAIL(ret)) {
        // SD_TRACE("Error sending SEND_SCR");
        return false;
    }
    
    // Determine card version
    // Note that the SCR is big-endian
    uint32_t scr0 = __builtin_bswap32(ret->scr->scr[0]);
    
    ret->scr->sd_version = SD_VER_UNKNOWN;
    
    uint32_t sd_spec = (scr0 >> (56 - 32)) & 0xf;
    uint32_t sd_spec3 = (scr0 >> (47 - 32)) & 0x1;
    uint32_t sd_spec4 = (scr0 >> (42 - 32)) & 0x1;
    
    ret->scr->sd_bus_widths = (scr0 >> (48 - 32)) & 0xf;
    
    if (sd_spec == 0) {
        ret->scr->sd_version = SD_VER_1;
    } else if (sd_spec == 1) {
        ret->scr->sd_version = SD_VER_1_1;
    } else if (sd_spec == 2) {
        if (sd_spec3 == 0) {
            ret->scr->sd_version = SD_VER_2;
        } else if (sd_spec3 == 1) {
            if (sd_spec4 == 0) {
                ret->scr->sd_version = SD_VER_3;
            } else if (sd_spec4 == 1) {
                ret->scr->sd_version = SD_VER_4;
            }
        }
    }
    
    PUT32(EMMC_INTERRUPT, 0xFFFFFFFF);
    
    return true;
}

bool EMMC::isSDCardInserted() {
    return GPIO::level(GPIO_CD);
}

uint32_t EMMC::version() {
    const uint32_t ver = GET32(EMMC_SLOTISR_VER);
    uint32_t SDVersion = (ver & EMMC_SLOTISR_VER_SDVERSION_MASK) >> EMMC_SLOTISR_VER_SDVERSION_SHIFT;
    return SDVersion;
}
