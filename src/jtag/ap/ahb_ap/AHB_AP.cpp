/*
 * AHB_AP.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "AccessPort.h"

namespace jtag {

AHB_AP::AHB_AP (std::string name) {

        this->name = name;
}

AHB_AP::~AHB_AP () {
        // TODO Auto-generated destructor stub
}

AHB_AP::select (AccessPort* access_port) {

  this->current_ap = access_port;

}

}
