//
//  DMA.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/12.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <extern.h>
#include <raspberrypi/DMA.h>

// DEBUG
#include <raspberrypi/UART.h>

using namespace raspberryPi;

struct CB control_blocks __attribute__ ((aligned (256)));

void DMA::enableEngine(uint32_t DMAEngine) {
    if (DMAEngine > 14) return;
    
    uint32_t enable = GET32(DMA_ENABLE);
    enable |= 1 << DMAEngine;
    PUT32(DMAEngine, enable);
}

void DMA::reset(uint32_t channel) {
    if (channel > 14) return;
    uint32_t reset = GET32(DMA_CS_REG(channel));
    reset = (1 << 31);
    PUT32(DMA_CS_REG(channel), reset);
}

void DMA::set(uint32_t channel, bool on) {
    if (channel > 14) return;
    
    uint32_t active = GET32(DMA_CS_REG(channel));
    if (on) active |= 0x1;
    else active &= ~(0x1);
    PUT32(DMA_CS_REG(channel), active);
}

void DMA::clearInterrupt(uint32_t channel) {
    if (channel > 14) return;
    
    uint32_t active = GET32(DMA_CS_REG(channel));
    active &= ~(0x4);
    PUT32(DMA_CS_REG(channel), active);
}

void DMA::clearEnd(uint32_t channel) {
    if (channel > 14) return;
    
    uint32_t active = GET32(DMA_CS_REG(channel));
    active &= ~(0x2);
    PUT32(DMA_CS_REG(channel), active);
}

void DMA::memcpy(uint32_t *src, uint32_t *dest, uint32_t length, uint32_t usingChannel) {
    control_blocks.src = *src;
    control_blocks.dest = *dest;
    control_blocks.length = length;
    control_blocks.next = 0;
    control_blocks.mode2D = length;
    
    // No interrupt
    // No 2D mode
    // Don't wait
    // Dest inc 32
    // Dest width 128
    // No dest dreq
    // Write
    // Src inc 32
    // Src width 128
    // No src dreq
    // Read
    // burst length 8 words
    // No premap
    // No waits
    // Dont do wide writes as a 2 beat burst
    control_blocks.info = 0x1008338;
}

void DMA::sendDMARequest(struct CB *conblk, uint32_t toChannel) {
    // TODO: wait if not done
    
    PUT32(DMA_CONBLK_AD_REG(toChannel), (uint32_t)conblk);
    PUT32(DMA_DEBUG_REG(toChannel), 0);
    set(toChannel);
}
