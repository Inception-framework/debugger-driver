/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */
#include <stdint.h>

#define _LOG_ALL
#include "colored.h"

#include "jtag/CommandsFactory.h"
#include "jtag/Command.h"

#include "interface/Interface.h"
#include "interface/Producer.h"
#include "interface/Consumer.h"


using namespace jtag;

int main(int argc, char* argv[]) {

	INFO("Init","Avatar gateway version 1.0");

	Command* cmd;
	uint8_t stopped = NULL;

	INFO("Device","Initializing superspeed device ...");
	Device::USBDevice* fx3 = new Device::USBDevice (0x04B4, 0x00F0, 0);
	fx3->init();

	INFO("Interface","Starting producer thread ...");
	Interface* producer = new Producer (fx3);
	producer->start ();

	INFO("Interface","Starting consumer thread ...");
	Interface* consumer = new Consumer (fx3);
	consumer->start ();

	/*
	* The producer thread only sends JTAG requests
	* The consumer thread read tdo ouput and process result : extract data from jtag protocol
	*
	* A Command object contains a binary array which can be directly send to the device
	* So If a big file has to be send, one command object is give to the Producer that sends it following the USB3 limitation
	*/

	INFO("Command","Creating RESET command ...");
	cmd = CommandsFactory::CreateCommand (COMMAND_TYPE::RESET, 0, 0);
	producer->add_cmd_to_queue (cmd);
	producer->process_jtag_queue ();

	INFO("Command","Creating WRITE_U32 command ...");
	cmd = CommandsFactory::CreateCommand (COMMAND_TYPE::WRITE_U32, 0xABABABAB, 0x20000000);
	producer->add_cmd_to_queue (cmd);
	producer->process_jtag_queue ();

	INFO("Command","Creating IDCODE command ...");
	cmd = CommandsFactory::CreateCommand (COMMAND_TYPE::IDCODE, 0, 0);
	producer->add_cmd_to_queue (cmd);
	producer->process_jtag_queue ();

	INFO("User", "Press any key to shutdown Avatar");
	while (stopped == NULL) {
		std::cin >> stopped;
	}


	INFO("Interface","Shutting down interfaces ...");
	producer->stop ();
	//producer->wait();

	consumer->stop ();

	INFO("Device","Closing device connection ...");
	fx3->quit();

	INFO("main","Avatar was properly closed");
}
