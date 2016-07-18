/*
 * Producer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Producer.h"

int Producer::sent = 0;

Producer::Producer (Device::USBDevice* device) : Interface(device) {

								this->is_running = false;

}

Producer::~Producer () {
								// TODO Auto-generated destructor stub
}

void Producer::add_cmd_to_queue (jtag::Command* cmd) {

								this->queue.push(cmd);

}

void Producer::start () {

								this->task = std::thread(&Producer::process_jtag_queue, this);

								this->is_running = true;

}

void Producer::stop () {

								this->is_running = false;

}

void Producer::process_jtag_queue (void) {

								jtag::Command* cmd = NULL;
								//uint32_t size;

								while( this->is_running == true ) {

																/* The producer goal is to use the maximum of the USB3 bandwidth */
																while( (!this->queue.empty()) ) {

																								cmd = this->queue.front();

																								this->queue.pop();

																								INFO("Interface","Sending command %s ...", cmd->command_name());

																								//if ( size == 1024 )
																								this->device.download (cmd->get_buffer () );

																								Producer::sent++;
																}
								}
}
