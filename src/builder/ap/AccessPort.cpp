/*
 * Command.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "AccessPort.h"

namespace jtag {

AccessPort *AccessPort::current_ap = NULL;

AccessPort::AccessPort(std::string name) { this->name = name; }

AccessPort::~AccessPort() {
  // TODO Auto-generated destructor stub
}

void AccessPort::select(AccessPort *access_port) {

  AccessPort::current_ap = access_port;
}
}
