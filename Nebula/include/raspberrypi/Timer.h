//
//  Timer.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __TIMER_H
#define __TIMER_H

#include <sys/types.h>

#pragma mark
#pragma mark - Definitions

/**
 *  @brief  Timer Base Address
 *
 *  @discussion The ARM Timer is based on a ARM AP804, but it has a number of differences with the standard SP804:
 *       • There is only one timer.
 *       • It only runs in continuous mode.
 *       • It has a extra clock pre-divider register.
 *       • It has a extra stop-in-debug-mode control bit.
 *       • It also has a 32-bit free running counter.
 *   The clock from the ARM timer is derived from the system clock. This clock can change dynamically e.g. if the system goes into reduced power or in low power mode. Thus the clock speed adapts to the overal system performance capabilities. For accurate timing it is recommended to use the system timers.
 *
 */
#define ARM_TIMER_BASE     (ARM_PERIPHERALS_BASE + 0xB400)

/**
 *  @brief  Timer Load register
 *
 *  @discussion The timer load register sets the time for the timer to count down. This value is loaded into the timer value register after the load register has been written or if the timer-value register has counted down to 0.
 *
 */
#define ARM_TIMER_LOAD     (ARM_TIMER_BASE + 0x00)

/**
 *  @brief  Timer Value register
 *
 *  @discussion This register holds the current timer value and is counted down when the counter is running. It is counted down each timer clock until the value 0 is reached. Then the value register is re-loaded from the timer load register and the interrupt pending bit is set. The timer count down speed is set by the timer pre-divide register.
 *
 */
#define ARM_TIMER_VALUE    (ARM_TIMER_BASE + 0x04)

/**
 *  @brief  Timer control register
 *
 *  @discussion The standard SP804 timer control register consist of 8 bits but in the BCM implementation there are more control bits for the extra features. Control bits 0-7 are identical to the SP804 bits, albeit some functionality of the SP804 is not implemented. All new control bits start from bit 8 upwards. Differences between a real 804 and the BCM implementation are shown in [].
 *
 *   Bit(s)   R/W       Function
 *   31:10     -        <Unused>
 *   23:16    R/W       Free running counter pre-scaler. Freq is sys_clk/(prescale+1)
 *                      [These bits do not exists in a standard 804! Reset value is 0x3E]
 *   15:10     -        <Unused>
 *     9      R/W       0 : Free running counter Disabled
 *                      1 : Free running counter Enabled
 *                      [This bit does not exists in a standard 804 timer!]
 *     8      R/W       0 : Timers keeps running if ARM is in debug halted mode
 *                      1 : Timers halted if ARM is in debug halted mode
 *                      [This bit does not exists in a standard 804 timer!]
 *     7      R/W       0 : Timer disabled
 *                      1 : Timer enabled
 *     6      R/W       [Not used], The timer is always in free running mode.
 *                      If this bit is set it enables periodic mode in a standard 804. That mode is not supported in the BCM2835/BCM2836.
 *     5      R/W       0 : Timer interrupt disabled
 *                      1 : Timer interrupt enabled
 *     4      R/W       <Not used>
 *    3:2     R/W       Pre-scale bits:
 *                      00 : pre-scale is clock / 1 (No pre-scale)
 *                      01 : pre-scale is clock / 16
 *                      10 : pre-scale is clock / 256
 *                      11 : pre-scale is clock / 1 (Undefined in 804)
 *     1      R/W       0 : 16-bit counters
 *                      1 : 23-bit counter
 *     0      R/W       [Not used], The timer is always in wrapping mode.
 *                      If this bit is set it enables one-shot mode in real 804. That mode is not supported in the BCM2835/BCM2836.
 *
 */
#define ARM_TIMER_CTL      (ARM_TIMER_BASE + 0x08)

/**
 *  @brief  Timer IRQ clear register
 *
 *  @discussion The timer IRQ clear register is write only. When writing this register the interrupt-pending bit is cleared.
 *
 *   When reading this register it returns 0x544D5241 which is the ASCII reversed value for "ARMT".
 *
 */
#define ARM_TIMER_IRQ_CLR  (ARM_TIMER_BASE + 0x0C)
#define ARM_TIMER_ACK      (ARM_TIMER_BASE + 0x0C)

/**
 *  @brief  Timer Raw IRQ register
 *
 *  @discussion The raw IRQ register is a read-only register. It shows the status of the interrupt pending bit.
 *
 *   Bit(s)   R/W       Function
 *   31:0      R        0
 *    0        R        0 : The interrupt pending bits is clear
 *                      1 : The interrupt pending bit is set.
 *
 *   The interrupt pending bits is set each time the value register is counted down to zero. The interrupt pending bit can not by itself generates interrupts. Interrupts can only be generated if the interrupt enable bit is set.
 *
 */
#define ARM_TIMER_RAW_IRQ  (ARM_TIMER_BASE + 0x10)

/**
 *  @brief  Timer Masked IRQ register
 *
 *  @discussion The masked IRQ register is a read-only register. It shows the status of the interrupt signal. It is simply a logical AND of the interrupt pending bit and the interrupt enable bit.
 *
 *   Bit(s)   R/W       Function
 *   31:0      R        0
 *    0        R        0 : Interrupt line not asserted.
 *                      1 :Interrupt line is asserted, (the interrupt pending and the interrupt
 enable bit are set.)
 *
 */
#define ARM_TIMER_MASK_IRQ (ARM_TIMER_BASE + 0x14)

/**
 *  @brief  Timer Reload register
 *
 *  @discussion This register is a copy of the timer load register. The difference is that a write to this register does not trigger an immediate reload of the timer value register. Instead the timer load register value is only accessed if the value register has finished counting down to zero.
 *
 */
#define ARM_TIMER_RELOAD   (ARM_TIMER_BASE + 0x18)

/**
 *  @brief  The timer pre-divider register
 *
 *  @discussion The timer load register sets the time for the timer to count down. This value is loaded into the timer value register after the load register has been written or if the timer-value register has counted down to 0.
 *
 *   Bit(s)   R/W       Function
 *   31:10      R       0
 *    9:0       R       Pre-divider value.
 *
 *   The Pre-divider register is not present in the SP804.
 *
 *   The pre-divider register is 10 bits wide and can be written or read from. This register has been added as the SP804 expects a 1MHz clock which we do not have. Instead the pre-divider takes the APB clock and divides it down according to:
 *      timer_clock = apb_clock/(pre_divider+1)
 *   The reset value of this register is 0x7D so gives a divide by 126.
 *
 */
#define ARM_TIMER_PRE_DIV  (ARM_TIMER_BASE + 0x1C)

/**
 *  @brief  Free running counter
 *
 *  @discussion The timer load register sets the time for the timer to count down. This value is loaded into the timer value register after the load register has been written or if the timer-value register has counted down to 0.
 *
 *   Bit(s)   R/W       Function
 *   31:0      R        Counter value
 *
 *   The free running counter is not present in the SP804.
 *   The free running counter is a 32 bits wide read only register. The register is enabled by setting bit 9 of the Timer control register. The free running counter is incremented immediately after it is enabled. The timer can not be reset but when enabled, will always increment and roll-over. The free running counter is also running from the APB clock and has its own clock pre-divider controlled by bits 16-23 of the timer control register.
 *   This register will be halted too if bit 8 of the control register is set and the ARM is in Debug Halt mode.
 *
 */
#define ARM_TIMER_CNT      (ARM_TIMER_BASE + 0x20)

namespace raspberryPi {
    namespace Timer {
        void init();
        uint32_t tick();
    }
}

#endif /* __TIMER_H */
