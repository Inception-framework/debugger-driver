/*
 * Command.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "AccessPort.h"

namespace jtag {

AccessPort* Jtag::current_ap = NULL;

AccessPort::AccessPort (std::string name) {

        this->name = name;
}

AccessPort::~AccessPort () {
        // TODO Auto-generated destructor stub
}

AccessPort::select (AccessPort* access_port) {

  this->current_ap = access_port;

}

}
