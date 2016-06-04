//
//  Memory.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/9.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/raspberryPi.h>
#include <raspberrypi/Memory.h>

using namespace raspberryPi;

#define mem_p2v(X) (X+0x80000000)
static unsigned int *pagetable = (unsigned int * const) mem_p2v(0x4000);
//
///* Translation table 0 - covers the first 64 MB, for now
// * Needs to be aligned to its size (ie 64*4 bytes)
// */
//unsigned int pagetable0[64]	__attribute__ ((aligned (256)));
//
//void Memory::init() {
//    unsigned int x;
//    unsigned int pt0_addr;
//    
//    /* Translation table 0 - covers the first 64 MB, for now
//     * Currently nothing mapped in it.
//     */
//    for(x=0; x<64; x++)
//    {
//        pagetable0[x] = 0;
//    }
//    
//    /* Get physical address of pagetable0 */
//    pt0_addr = addressToPhysical((unsigned int) &pagetable0);
//    
//    /* Use translation table 0 up to 64MB */
//    asm volatile("mcr p15, 0, %[n], c2, c0, 2" : : [n] "r" (6));
//    /* Translation table 0 - ARM1176JZF-S manual, 3-57 */
//    asm volatile("mcr p15, 0, %[addr], c2, c0, 0" : : [addr] "r" (pt0_addr));
//    /* Invalidate the translation lookaside buffer (TLB)
//     * ARM1176JZF-S manual, p. 3-86
//     */
//    asm volatile("mcr p15, 0, %[data], c8, c7, 0" : : [data] "r" (0));
//}

volatile void Memory::barrier() {
    asm volatile( "mcr p15, 0, ip, c7, c5, 0" );
    asm volatile( "mcr p15, 0, ip, c7, c5, 6" );
    asm volatile( "mcr p15, 0, ip, c7, c10, 4" );
    asm volatile( "mcr p15, 0, ip, c7, c5, 4" );
}

uint32_t Memory::addressToPhysical(uint32_t addr) {
    unsigned int pt_data = pagetable[addr >> 20];
    unsigned int cpt_data, physaddr;
    
    if((pt_data & 3)==0 || (pt_data & 3)==3)
    {
        /* Nothing mapped */
        return 0xffffffff;
    }
    
    if((pt_data & 3)==2)
    {
        /* (Super)Section */
        
        physaddr = pt_data & 0xfff00000;
        
        if(pt_data & (1<<18))
        {
            /* 16MB Supersection */
            physaddr += addr & 0x00ffffff;
        }
        else
        {
            /* 1MB Section */
            physaddr += addr & 0x000fffff;
        }
        
        return physaddr;
    }
    
    /* Coarse page table */
    cpt_data = ((unsigned int *)(0x80000000 + (pt_data & 0xfffffc00)))[(addr>>12)&0xff] ;
    
    if((cpt_data & 3) == 0)
    {
        /* Nothing mapped */
        return 0xffffffff;
    }
    
    if(cpt_data & 2)
    {
        /* Small (4k) page */
        return (cpt_data & 0xfffff000) + (addr & 0xfff);
    }
    
    /* Large (64k) page */
    return (cpt_data & 0xffff0000) + (addr & 0xffff);
}

uint32_t Memory::addressToVirtual(uint32_t addr) {
    return (unsigned int)(addr + 0x80000000);
}

void Memory::memclr(void * address, unsigned int length) {
    register unsigned int addr = (unsigned int)address;
    
    /* If the start address is unaligned, fill in the first 1-3 bytes
     * until it is
     */
    while((addr & 3) && length) {
        *((unsigned char *)addr) = 0;
        addr++;
        length--;
    }
    
    /* Fill in the remaining 32-bit word-aligned memory locations */
    while(length & 0xFFFFFFFC) {
        *((unsigned int *)addr) = 0;
        addr += 4;
        length -= 4;
    }
    
    /* Deal with the remaining 1-3 bytes, if any */
    while(length) {
        addr++;
        length--;
        *((unsigned char *)addr) = 0;
    }
}

void Memory::memcpy(uint32_t * dest, uint32_t * src, uint32_t length) {
    if (length == 0) return;
    
    for (uint32_t i = 0; i < length; i++) {
        dest[i] = src[i];
    }
}

void * Memory::memmove(void * dest, const void * src, unsigned int length) {
    /* Turn destination and source pointers into integers for easier
     * calculations
     */
    register unsigned int d = (unsigned int)dest;
    register unsigned int s = (unsigned int)src;
    
    if(!length) return dest;
    
    /* Assume the memory blocks are word aligned. Most will be, and the
     * CPU can deal with unaligned accesses if necessary (as long as it
     * is configured to do so)
     */
    if(d>s && d<(s+length)) {
        /* Destination starts inside source area - work backwards */
        /* If length isn't a multiple of 4 bytes, copy the last 1-3
         * bytes first
         */
        while(length & 3) {
            length--;
            ((unsigned char *)d)[length] = ((unsigned char *)s)[length];
        }
        
        /* Copy everything else as 32-bit words. If one or both of the
         * memory areas aren't aligned, this will cause unaligned
         * reads. Inefficient, but less so than doing everything as
         * a series of bytes
         */
        while(length) {
            length-=4;
            *((unsigned int *)d+length) = *((unsigned int *)s+length);
        }
    } else {
        /* Source starts inside destination area - working forwards
         * is fine - or two areas don't overlap (or they overlap
         * exactly, but that's an unlikely edge case)
         *
         * Copy as much as possible as 32-bit words. See above for
         * alignment issues
         */
        while(length & 0xfffffffc) {
            *((unsigned int *)d) = *((unsigned int *)s);
            d+=4;
            s+=4;
            length-=4;
        }
        
        /* Deal with 1-3 remaining bytes, if applicable */
        while(length) {
            *((unsigned char *)d) = *((unsigned char *)s);
            d++;
            s++;
            length--;
        }
    }
    
    return dest;
}

