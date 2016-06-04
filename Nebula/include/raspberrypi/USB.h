//
//  USB.hpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __USB_H
#define __USB_H

#include <sys/types.h>

namespace raspberryPi {
    namespace USB {
        /**
         *  @brief  初始化USB控制器
         *
         *  @discussion 这里会检查并自动启用USB控制器
         */
        void init();
    }
}

#endif /* __USB_H */
