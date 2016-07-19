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

								this->lock ();

								this->queue.push (cmd);

								puts(" Command added to the queue");

								this->unlock ();
}

void Producer::start () {

								this->is_running = true;

								//this->task = std::thread (&Producer::process_jtag_queue, this);

}

void Producer::stop () {

								this->is_running = false;

}

void Producer::process_jtag_queue (void) {

								jtag::Command* cmd = NULL;
								//uint32_t size;s

								INFO("Interface"," Producer started");

								//while( this->is_running == true ) {

																/* The producer goal is to use the maximum of the USB3 bandwidth */
																if( this->queue.empty () == false ) {

																								puts("Producer processes command");

																								this->lock ();

																								cmd = this->queue.front ();

																								printf("\r\n[*] Sending command %s %dB...\n", cmd->command_name (), cmd->size ());
																								for (int i=0; i<cmd->size (); i++)
																									printf("%02x", cmd->get_buffer ()[i]);
																								printf("\r\n");

																								//if ( size == 1024 )
																								this->device->download (cmd->get_buffer (), cmd->size () );

																								this->queue.pop ();

																								this->unlock ();

																								delete cmd;

																								Producer::sent++;
																}
								//}
}
