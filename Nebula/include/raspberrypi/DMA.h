//
//  DMA.hpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/12.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __DMA_H
#define __DMA_H

#include <sys/types.h>

#define DMA_BASE                (ARM_PERIPHERALS_BASE + 0x00007000)

#define DMA_CHANNEL(x)          (DMA_BASE + 0x0100 * x)
#define DMA_CHANNEL_15          0x3FE05000

#define DMA_CS_REG(x)           (DMA_CHANNEL(x) + 0x00)
#define DMA_CONBLK_AD_REG(x)    (DMA_CHANNEL(x) + 0x04)
#define DMA_TI_REG(x)           (DMA_CHANNEL(x) + 0x08)
#define DMA_SOURCE_AD_REG(x)    (DMA_CHANNEL(x) + 0x0C)
#define DMA_DEST_AD_REG(x)      (DMA_CHANNEL(x) + 0x10)
#define DMA_TXFR_LEN(x)         (DMA_CHANNEL(x) + 0x14)
#define DMA_STRIDE_REG(x)       (DMA_CHANNEL(x) + 0x18)
#define DMA_NEXTCONBLK_REG(x)   (DMA_CHANNEL(x) + 0x1C)
#define DMA_DEBUG_REG(x)        (DMA_CHANNEL(x) + 0x20)

#define DMA_INT_STATUS          (DMA_BASE + 0x0FE0)
#define DMA_ENABLE              (DMA_BASE + 0x0FF0)

struct CB {
    uint32_t info;
    uint32_t src;
    uint32_t dest;
    uint32_t length;
    uint32_t mode2D;
    uint32_t next;
    uint32_t reserved1;
    uint32_t reserved2;
};

namespace raspberryPi {
    /**
     *  @brief  DMA Engine
     *
     *  @discussion 如果要使用DMA, 按照如下调用顺序
     *              (1) enableEngine        -> 启用某一个DMA
     *              (2) clearInterrupt      -> 清除之前的中断标识
     *              (3) 填写control_blocks   -> 用于向DMA传递信息
     *              (4) sendDMARequest      -> 真正发起请求
     *
     *              DMA大体流程(以channel 0为例):
     *              (a) enableEngine(1)
     *              (b) clearInterrupt(1)
     *              (c) clearEnd(1)
     *              (d) 填写control_blocks
     *              (e) sendDMARequest(control_blocks)
     *              (f) |- 0_CONBLK_AD被写入control_blocks的地址
     *              (f) |- 0_TI被写入传输信息
     *              (f) |- 0_SOURCE_AD被写入源地址(起始)
     *              (f) |- 0_DEST_AD被写入目的地址(起始)
     *              (f) |- 0_TXFR_LEN被写入传输长度, 非2D模式只有低16位有效, 即非2D模式一次最多传输65535 bytes
     *              (f) |- 0_STRIDE被写入2D模式下的相关信息
     *              (f) |- DMA_NEXTCONBLK_REG被写入下一个control_blocks的地址, 没有则写入0
     *              (f) |- 0_DEBUG被写入debug标识
     *
     */
    namespace DMA {
        void enableEngine(uint32_t DMAEngine);
        void reset(uint32_t channel);
        void set(uint32_t channel, bool on = true);
        void clearInterrupt(uint32_t channel);
        void clearEnd(uint32_t channel);
        /**
         *  @brief  快速memcpy
         *
         *  @param src          源地址
         *  @param dest         目的地址
         *  @param length       长度(byte)
         *  @param usingChannel DMA通道
         *
         *  @discussion 当目的地址是ARM外围设备时, 需要该设备的物理地址
         */
        void memcpy(uint32_t * src, uint32_t * dest, uint32_t length, uint32_t usingChannel = 0);
        void sendDMARequest(struct CB * conblk, uint32_t toChannel);
    }
}


#endif /* __DMA_H */
