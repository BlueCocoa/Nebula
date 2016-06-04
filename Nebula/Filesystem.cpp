//
//  Filesystem.cpp
//  Nebula
//
//  Created by BlueCocoa on 15/9/11.
//  Copyright © 2015年 0xBBC. All rights reserved.
//

#include <raspberrypi/Filesystem.h>
#include <raspberrypi/EMMC.h>

using namespace raspberryPi;

void Filesystem::initOnBoardSDCard(Filesystem::filesystem_type_t type) {
    if (type == FAT) {
        if (EMMC::reset()) {
            
        }
    }
}
