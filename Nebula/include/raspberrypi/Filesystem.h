//
//  Filesystem.h
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <sys/types.h>

namespace raspberryPi {
    namespace Filesystem {
        enum filesystem_type_t {
            FAT     = 0x86
        };
        
        /**
         *  @brief 初始化树莓派上用于启动的SD Card, 并非外接的SD卡
         */
        void initOnBoardSDCard(filesystem_type_t type = FAT);
    }
}

#endif /* __FILESYSTEM_H */
