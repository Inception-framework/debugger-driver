/*
 * Interface.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Interface.h"


Interface::Interface (Device::USBDevice* device) {

  this->device = device;
}

Interface::~Interface () {
	// TODO Auto-generated destructor stub
}
