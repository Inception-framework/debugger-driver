/*
 * AHB_AP.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "JtagAP.h"

namespace jtag {

JtagAP::JtagAP() : AccessPort("JTAG AP") { this->select_register = 0x00000000; }

JtagAP::~JtagAP() {
  // TODO Auto-generated destructor stub
}

uint32_t JtagAP::select() {

  AccessPort::select(this);

  return this->select_register;
}
}
