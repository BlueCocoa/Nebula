// start.s
// Version v0.0.1
// Build 63
// Created by BlueCocoa on 15/8/12.
// Copyright © 2015. All rights reserved.
// Description Bootstrap

#include "start.h"

.section .init
.extern createPageTables
.extern os_irq_handler
.globl _start
_start:
    mov r4, #0x80000000
    // SVC stack
    mrs    r0, cpsr
    bic    r0, r0, #0x1F //0b11111
    orr    r0, r0, #0xd3 //0b10011
    msr    cpsr,r0
    add sp, r4, #0x2400

//    // Abort stack
//    mrs    r0, cpsr
//    bic    r0, r0, #0x1F //0b11111
//    orr    r0, r0, #0x17 //0b10111
//    msr    cpsr,r0
//    mov sp, #0x2800
//
//    // IRQ stack
//    mrs    r0, cpsr
//    bic    r0, r0, #0x1F //0b11111
//    orr    r0, r0, #0x12 //0b10010
//    msr    cpsr,r0
//    mov sp, #0x2C00

    // Sys stack
    mrs    r0, cpsr
    bic    r0, r0, #0x1F //0b11111
    orr    r0, r0, #0x1F //0b11111
    msr    cpsr,r0
    add sp, r4, #0x3C00

    /* 创建页表 */
    bl createPageTables

    b hang
hang: b hang

//.section .high
//.globl led
//led:
//    bl __GET32
//    mov r3, r0          //
//    ldr r0, =0xBF200000
//    cmp r3, r0
//    bne end$
//    blink$:
//        mov r1,#1
//        lsl r1,#21
//        str r1,[r0,#0x10]
//        mov r1,#1
//        lsl r1,#15
//        mov r3, #3
//        loop$:
//            str r1,[r0,#0x20]
//            mov r2,#0x4F0000
//            wait1$:
//                sub r2,#1
//                cmp r2,#0
//                bne wait1$
//
//            str r1,[r0,#0x2C]
//            mov r2,#0x1F0000
//            wait2$:
//                sub r2,#1
//                cmp r2,#0
//                bne wait2$
//
//            sub r3, #1
//            cmp r3, #0
//            bne loop$
//    end$:
//    bx lr

//.globl irq_set
//irq_set:
//    ldr pc, reset_handler
//    ldr pc, undefined_handler
//    ldr pc, swi_handler
//    ldr pc, prefetch_handler
//    ldr pc, data_handler
//    ldr pc, unused_handler
//    ldr pc, irq_handler
//    ldr pc, fiq_handler
//reset_handler:      .word reset
//undefined_handler:  .word hang
//swi_handler:        .word irq
//prefetch_handler:   .word hang
//data_handler:       .word irq
//unused_handler:     .word hang
//irq_handler:        .word irq
//fiq_handler:        .word irq
//reset:
//    mov r0, #0x8000
//    mov r1, #0x0000
//    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
//    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
//    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
//    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
//
//
//    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
//    mov r0,#0xD2
//    msr cpsr_c,r0
//    mov sp,#0x8000
//
//    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
//    mov r0,#0xD1
//    msr cpsr_c,r0
//    mov sp,#0x4000
//
//    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
//    mov r0,#0xD3
//    msr cpsr_c,r0
//    mov sp,#0x8000000
//
//    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
//    ;@mov r0,#0x53
//    ;@msr cpsr_c, r0
//
//.globl irq_enable
//irq_enable:
//    mrs r0, cpsr
//    bic r0, r0, #0x80
//    msr cpsr_c ,r0
//    bx lr
//
//irq:
//    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
//    bl os_irq_handler
//    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
//    subs pc, lr, #4

.section .system-wide

//// C -> void PUT32(unsigned int address, unsigned int value);
//.globl __PUT32
//__PUT32:
//    add r0, #0xBF000000
//    str r1,[r0]
//    bx lr
//
//// C -> unsigned int _GET32(unsigned int address);
//.globl __GET32
//__GET32:
//    add r0, #0xBF000000
//    ldr r0, [r0]
//    bx lr

// C -> void wait_op(void);

.globl wait_op
wait_op:
    bx lr

.globl waitAF
waitAF:
    mov r2,#0x1F0000
    wait2A$:
        sub r2,#1
        cmp r2,#0
        bne wait2A$
    bx lr

.section .rawdata
.incbin "NAO.dat"
