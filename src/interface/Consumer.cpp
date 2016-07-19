/*
 * Consumer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Consumer.h"

Consumer::Consumer (Device::USBDevice* device)  : Interface(device) {

  this->is_running = false;
}

Consumer::~Consumer () {
	// TODO Auto-generated destructor stub
}

void Consumer::add_cmd_to_queue (jtag::Command* cmd) {

}

void Consumer::start () {

  this->is_running = true;

  this->task = std::thread(&Consumer::process_jtag_queue, this);

}

void Consumer::stop () {
  this->is_running = false;
}

void Consumer::process_jtag_queue (void) {

  while( this->is_running == true ) {

  }

}
