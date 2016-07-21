/*
 * AHB_AP.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "AHB_AP.h"

namespace jtag {

AHB_AP::AHB_AP () : AccessPort("AHB_AP") {

        this->select_register = 0x00000010;

}

AHB_AP::~AHB_AP () {
        // TODO Auto-generated destructor stub
}

uint32_t AHB_AP::select () {

        AccessPort::select (this);


        return this->select_register;

}

}
