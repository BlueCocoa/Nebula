////
////  initsys.c
////  Nebula
////
////  Created by BlueCocoa on 15/9/13.
////  Copyright © 2015年 0xBBC. All rights reserved.
////
//

//
///**
// *  @discussion
// *      我们一共有三个地址,
// *          (1) VideoCore Bus Addresses 0x00000000 - 0xFFFFFFFF
// *          (2) ARM Physical Addresses  0x00000000 - 0x40000000
// *          (3) ARM Virtual Addresses   0x00000000 - 0xFFFFFFFF
// *      页表的创建是为了管理(3) ARM Virtual Addresses
// *      后文中使用的标注方式:
// *              0xFFFFFFFF(1), 表示我们在谈论VideoCore Bus Addresses中的0xFFFFFFFF
// *              0x3F000000(2), 表示我们在谈论ARM Physical Addresses中的0x3F000000
// *              0x7FFFFFFF(3), 表示我们在谈论ARM Virtual Addresses中的0x7FFFFFFF
// *              0x00ABCDEF, 表示这个16进制数, 而非地址
// *      后文如果涉及到地址范围, 若无特别说明则均为闭区间
// *
// *      首先,
// *          VideoCore将总线地址0xC0000000(1)映射为ARM上的物理地址, ARM物理地址起始点为0x00000000(2)
// *          从0x00000000(2)开始, 第一部分为ARM使用的内存, 第二部分为VideoCore使用的内存(即显存), 第一、二部分加在一起等于RAM的大小
// *          在系统总内存结束之后, 是I/O外围设备的地址
// *              不同型号的树莓派的I/O基地址如下
// *              Board Rev.      I/O Base
// *              0x00000002      ?
// *              0x00000003      ?
// *              0x00000004      ?
// *              0x00000005      ?
// *              0x00000006      ?
// *              0x00000007      ?
// *              0x00000008      ?
// *              0x00000009      ?
// *              0x0000000D      ?
// *              0x0000000E      ?
// *              0x0000000F      ?
// *              0x00000010      ?
// *              0x00000011      ?
// *              0x00000012      ?
// *              0x00A21041      0x3F000000(2)
// *              0x00A21041      0x3F000000(2)
// *
// *      接着来说说(3) ARM Virtual Addresses
// *          从0x00000000(3)开始, 一直到0xBFFFFFFF(3)都是User-mode的虚拟地址
// *          从0xC0000000(3)开始, 长度为树莓派RAM大小的区域为SDRAM和VideoCore SDRAM
// *          在这之后是kernel-mode的虚拟地址, 一直到0xF1FFFFFF(3)结束
// *          从0xF2000000(3)开始, 一直到0xF2FFFFFF(3)为I/O外围设备在ARM Virtual Addresses中的映射
// *
// *      在了解了树莓派的地址布局之后, 我们需要在linker script里作出相应的动作
// *
// *      我们设计的布局, (在树莓派2代 Model B上)
// *          此处十六进制取闭区间, 后面的十进制取左闭右开区间
// *          0x00000000(3) - 0x7FFFFFFF(3) (0MB    - 2048MB) = 用户进程内存 (用户只能访问到这个范围)
// *          0x80000000(3) - 0xBFFFFFFF(3) (2048MB - 3072MB) = 物理内存
// *              外围设备在0xBF000000(3) - 0xBFFFFFFF(3) (16MB)
// *          0xC0000000(3) - 0xEFFFFFFF(3) (3072MB - 3840MB) = 内核的heap/stack
// *          0xF0000000(3) - 0xFFFFFFFF(3) (3840MB - 4096MB) = 内核代码
// *
// *      我们知道, 当树莓派通电后, kernel.img被VideoCore加载进内存, 此时我们在(3) ARM Virtual Addresses中,
// *      我们的_start在0x00008000(3), sp也设定在0x00008000(3), 此时我们需要下面这个函数来执行内存分页, 然后再跳转到我们的kernel_start
// *
// */
//
///**
// *  @brief  一级描述符类型
// *
// *  @discussion
// *      低2位
// *      00 Fault    -   Generates a Section Translation Fault
// *      01 Page     -   Indicates that this is a Page Descriptor
// *          Bit(s) | Function
// *          31:10    Page table base address
// *           8:5     Domain, 共16个Domian可设置
// *            4      (always written as 1 for backward compatibility)
// *           3:2     (always written as 0)
// *      10 Section  -   Indicates that this is a Section Descriptor
// *          Bit(s) | Function
// *          31:19    Section base address
// *          11:10    AP, access permissions
// *                      AP  | S | R | Superivor |   User   | Notes
// *                      00    0   0   No Access   No Access  Any access generates a permission fault
// *                      00    0   1   Read-only   Read-only  Any write generates a permission fault
// *                      00    1   0   Read-only   No Access  Supervisor read only permitted
// *                      00    1   1       -           -
// *                      01    x   x   Read-write  No Access  Access allowed only in Supervisor mode
// *                      10    x   x   Read-write  Read-only  Writes in User mode cause permission fault
// *                      11    x   x   Read-write  Read-write All access types permitted in both modes.
// *           8:5     Domain, 共16个Domian可设置
// *            4      XN
// *            3      Enable Cache if is set to 1
// *            2      Enable Buffer
// *      11 Section  -   P eXecute Never
// */

#define MMU_L1_TYPE_INVALID       0b00
#define MMU_L1_TYPE_PAGE          0b01
#define MMU_L1_TYPE_SECTION       0b10
#define MMU_L1_TYPE_RESERVED      0b11

#define MMU_L1_SEC_SET_PA(x)      (x & 0xFFF00000)  // Section base address
#define MMU_L1_SEC_SET_NS(x)      (x << 19)  // Non-secure
#define MMU_L1_SEC_SET_nG(x)      (x << 17)  // non-Global
#define MMU_L1_SEC_SET_S(x)       (x << 16)  // Shareable
#define MMU_L1_SEC_SET_AP2(x)     (x << 15)
#define MMU_L1_SEC_SET_TEX(x)     (x << 12)
#define MMU_L1_SEC_SET_AP10(x)    (x << 10)
#define MMU_L1_SEC_SET_DOMAIN(x)  (x << 5)
#define MMU_L1_SEC_SET_XN(x)      (x << 4)
#define MMU_L1_SEC_SET_C(x)       (x << 3)
#define MMU_L1_SEC_SET_B(x)       (x << 2)

#define MMU_L1_PAGE_BA(x)         (x & 0x3FFFFF000)
#define MMU_L1_PAGE_SET_DOMAIN(x) (x << 5)
#define MMU_L1_PAGE_SET_SBZ4(x)   (x << 4)
#define MMU_L1_PAGE_SET_NS(x)     (x << 3)
#define MMU_L1_PAGE_SET_SBZ2(x)   (x << 2)

#define USING_ADVANCED_MEMORY_LAYOUT

static unsigned int * PageTable   = (unsigned int * const)0x4000; /* 16K */
static unsigned int * kernelStack = (unsigned int * const)0x3C00; /* 1K */;

extern "C" void kernel_start(unsigned int, unsigned int, unsigned int);
extern "C" void led();
extern unsigned int __bss_start, __bss_end, _physdatastart, kernel_load;

extern "C"
void createPageTables() {
//    asm volatile ( "dmb" ::: "memory" );

    // Disable AFE, TRE, I, Z, C
    unsigned int SCTLR;
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : "=r" (SCTLR) );
    SCTLR &= ~(0x30001804);
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : : "r" (SCTLR));
    
    // Disable M
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : "=r" (SCTLR) );
    SCTLR &= ~(0x00000001);
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : : "r" (SCTLR) );

    asm volatile ( "dmb" ::: "memory" );
    
    unsigned int base;
    unsigned int physical;
//#ifndef USING_ADVANCED_MEMORY_LAYOUT
//    for (base = 0; base < 1024-16; base++)
//    {
//        // section descriptor (1 MB)
//        // outer and inner write back, write allocate, not shareable (fast but unsafe)
//        //PageTable[base] = base << 20 | 0x0140E;
//        // outer and inner write through, no write allocate, shareable (safe but slower)
//        PageTable[base] = base << 20 | 0x1040A;
//    }
//    for (; base < 4096; base++)
//    {
//        // shared device, never execute
//        PageTable[base] = base << 20 | 0x10416;
//    }
//#else
    
    for (base = 0; base < 4096; base++) {
        PageTable[base] = 0x10416;
    }
    
    // 全局栈
    physical = 0x00000000;
    base = 0;
    PageTable[0] =  MMU_L1_SEC_SET_PA(physical) |0x1040A;//\
                    MMU_L1_SEC_SET_NS(0) |\
                    MMU_L1_SEC_SET_nG(0) |\
                    MMU_L1_SEC_SET_S(1) |\
                    MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(1) | MMU_L1_SEC_SET_B(0) |\
                    MMU_L1_SEC_SET_AP2(0) | MMU_L1_SEC_SET_AP10(2) |\
                    MMU_L1_SEC_SET_DOMAIN(0) |\
                    MMU_L1_SEC_SET_XN(0) |\
                    MMU_L1_TYPE_SECTION;
    
    // 用户内存
    physical = 0x00100000;
    base = 1;
    for (; base < 2048; base++) {
        PageTable[base] =   MMU_L1_SEC_SET_PA(physical) |0x10416;//\
                            MMU_L1_SEC_SET_NS(0) |\
                            MMU_L1_SEC_SET_nG(0) |\
                            MMU_L1_SEC_SET_S(1) |\
                            MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(1) | MMU_L1_SEC_SET_B(0) |\
                            MMU_L1_SEC_SET_AP2(0) | MMU_L1_SEC_SET_AP10(3) |\
                            MMU_L1_SEC_SET_DOMAIN(0) |\
                            MMU_L1_SEC_SET_XN(0) |\
                            MMU_L1_TYPE_SECTION;
                            physical += 0x100000;
    }
    
    // 物理内存
    physical = 0x00000000;
    base = 2048;
    for (; base < 3072 - 16; base++) {
        PageTable[base] =   MMU_L1_SEC_SET_PA(physical) | 0x1040A;//\
                            MMU_L1_SEC_SET_NS(0) |\
                            MMU_L1_SEC_SET_nG(0) |\
                            MMU_L1_SEC_SET_S(1) |\
                            MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(1) | MMU_L1_SEC_SET_B(0) |\
                            MMU_L1_SEC_SET_AP2(0) | MMU_L1_SEC_SET_AP10(3) |\
                            MMU_L1_SEC_SET_DOMAIN(0) |\
                            MMU_L1_SEC_SET_XN(0) |\
                            MMU_L1_TYPE_SECTION;
        physical += 0x100000;
    }
    
    // I/O外围
    physical = 0x3F000000;
    base = 3072 - 16;
    for (; base < 3072; base++) {
        PageTable[base] =   MMU_L1_SEC_SET_PA(physical) | 0x10416 ;//\
                            MMU_L1_SEC_SET_NS(0) |\
                            MMU_L1_SEC_SET_nG(0) |\
                            MMU_L1_SEC_SET_S(1) |\
                            MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(0) | MMU_L1_SEC_SET_B(1) |\
                            MMU_L1_SEC_SET_AP2(0) | MMU_L1_SEC_SET_AP10(3) |\
                            MMU_L1_SEC_SET_DOMAIN(0) |\
                            MMU_L1_SEC_SET_XN(1) |\
                            MMU_L1_TYPE_SECTION;
        physical += 0x100000;
    }
    
//    // data
//    physical = _physdatastart;
//    base = 3072;
//    for (; base < 3840; base++) {
//        PageTable[base] =   MMU_L1_SEC_SET_PA(physical) |0x1040A;//\
//                            MMU_L1_SEC_SET_NS(0) |\
//                            MMU_L1_SEC_SET_nG(0) |\
//                            MMU_L1_SEC_SET_S(1) |\
//                            MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(0) | MMU_L1_SEC_SET_B(1) |\
//                            MMU_L1_SEC_SET_AP2(0) | MMU_L1_SEC_SET_AP10(3) |\
//                            MMU_L1_SEC_SET_DOMAIN(0) |\
//                            MMU_L1_SEC_SET_XN(1) |\
//                            MMU_L1_TYPE_SECTION;
//        physical += 0x100000;
//    }
    
    // data
    PageTable[3072] = 1 | (unsigned int)kernelStack;
    
    /* Populate kerneldatatable - see ARM1176JZF-S manual, 6-40
     *
     * APX/AP bits for a page table entry are at bits 9 and 4&5. The
     * meaning is the same as for a section entry.
     * 0 01 (0x0010) =  read-write privileded modes, no access otherwise
     *
     * bits 0 and 1 determine the page type:
     * 0 = unmapped, translation fault
     * 1 = large page (64K)			(XN is bit 15)
     * 2 = small page (4K), executable	(XN is bit 0)
     * 3 = small page (4K), not-executable  (XN is bit 0)
     *
     * 256 entries, one for each 4KB in the 1MB covered by the table
     */
    for(unsigned x=0; x<256; x++)
    {
        /* &_physbssend is the physical address of the end of the
         * kernel data - somewhere between 0x00009000 and 1MB (any
         * more than that and this code will need rewriting...)
         */
        if(x <= ((unsigned int)&__bss_end >> 12))
            kernelStack[x] = ((unsigned int)&_physdatastart + (x<<12)) | 0x0438 | 2;
        else
            kernelStack[x] = 0;
    }
    
    // kernel
    physical = 0x00000000;
    base = 3840;
    for (; base < 4096; base++) {
        PageTable[base] =   MMU_L1_SEC_SET_PA(physical) |0x1040A;//\
        MMU_L1_SEC_SET_NS(0) |\
        MMU_L1_SEC_SET_nG(0) |\
        MMU_L1_SEC_SET_S(0) |\
        MMU_L1_SEC_SET_TEX(0) | MMU_L1_SEC_SET_C(1) | MMU_L1_SEC_SET_B(0) |\
        MMU_L1_SEC_SET_AP2(1) | MMU_L1_SEC_SET_AP10(1) |\
        MMU_L1_SEC_SET_DOMAIN(0) |\
        MMU_L1_SEC_SET_XN(0) |\
        MMU_L1_TYPE_SECTION;
        physical += 0x100000;
    }
    
//#endif
    
    unsigned int * bss = &__bss_start;
    while(bss < &__bss_end) {
        *bss = 0;
        bss++;
    }
    

    
    unsigned auxctrl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
    auxctrl |= 1 << 6;
    asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));
    
    unsigned int pt_addr = (unsigned int)PageTable;
    
    asm volatile("mcr p15, 0, %[addr], c2, c0, 0" : : [addr] "r" (0x4000));
    asm volatile("mcr p15, 0, %[addr], c2, c0, 1" : : [addr] "r" (0x4000));
    asm volatile("mcr p15, 0, %[n], c2, c0, 2" : : [n] "r" (0));
    
    asm volatile("mcr p15, 0, %0, c3, c0, 0" :: "r" (1));
    
    unsigned SCR;
    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : "=r" (SCR) );

    // Data Cache
    SCR |= 0x0004;
    // Branch Prediction
    SCR |= 0x0800;
    // Instruction Caches
    SCR |= 0x1000;

    asm volatile ( "mrc p15, 0, %0, c1, c0, 0" : : "r" (SCR) : "memory");
    
    unsigned control;
    asm volatile("mrc p15, 0, %[control], c1, c0, 0" : [control] "=r" (control));
    /* Turn on MMU */
    control |= 1;
    /* Write value back to control register */
    asm volatile("mcr p15, 0, %[control], c1, c0, 0" : : [control] "r" (control));
    

    asm volatile("mov lr, %[kernel_start]" : : [kernel_start] "r" ((unsigned int)&kernel_start) );
    asm volatile("bx lr");
}
//
