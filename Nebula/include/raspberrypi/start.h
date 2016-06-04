//
//  start.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __START_H
#define __START_H

#define MEGABYTE                0x100000

#define MEM_SIZE                (256 * MEGABYTE)

/**
 *  @brief  分给GPU的内存大小
 */
#define GPU_MEMORY_SIZE         (128 * MEGABYTE)

/**
 *  @brief  分给ARM的内存大小 (总内存大小 - 分给GPU的内存大小)
 */
#define ARM_MEMORY_SIZE         (MEM_SIZE - GPU_MEMORY_SIZE)

/**
 *  @brief  每一页的大小
 */
#define PAGE_SIZE               4096

/**
 *  @brief  内核最大大小
 */
#define KERNEL_MAX_SIZE         (2 * MEGABYTE)

/**
 *  @brief  内核栈大小
 */
#define KERNEL_STACK_SIZE       0x20000

/**
 *  @brief  异常的栈大小
 */
#define EXCEPTION_STACK_SIZE    0x8000

/**
 *  @brief  页表1的大小
 */
#define PAGE_TABLE1_SIZE        0x4000

/**
 *  @brief  保留页
 */
#define PAGE_RESERVED           (4 * MEGABYTE)

#define KERNEL_START        0x8000
#define KERNEL_END          0x208000 //    (KERNEL_START + KERNEL_MAX_SIZE) // 0x208000
#define KERNEL_STACK        0x228000 //    (KERNEL_END + KERNEL_STACK_SIZE) // 0x228000

#define CORES                   4
#define MEM_ABORT_STACK		0x70000 // (KERNEL_STACK + KERNEL_STACK_SIZE * (CORES-1) + EXCEPTION_STACK_SIZE)
#define MEM_IRQ_STACK		0x90000 // (MEM_ABORT_STACK + EXCEPTION_STACK_SIZE * (CORES-1) + EXCEPTION_STACK_SIZE)
#define MEM_PAGE_TABLE1		0xA8000 // (MEM_IRQ_STACK + EXCEPTION_STACK_SIZE * (CORES-1))

#endif /* __START_H */
