/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */
#include "main.h"
#include "jtag/Command_test.cpp"

using namespace jtag;

int main(int argc, char *argv[]) {

  INFO("Init", "Avatar gateway version 1.0");

  if (argc > 3) {

    CommandTest().test();

    return 0;
  }

  Command *cmd;
  AccessPort *ap;
  uint8_t stopped = 0;
  std::vector<uint32_t> arg;

  INFO("Device", "Initializing superspeed device ...");
  Device::USBDevice *fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  INFO("Interface", "Starting consumer thread ...");
  Consumer *consumer = new Consumer(fx3);
  consumer->start();

  INFO("Interface", "Starting producer thread ...");
  Producer *producer = new Producer(fx3, consumer);
  producer->start();

  INFO("Decoder", "Starting decoder thread ...");
  Decoder *decoder = new Decoder(producer);
  decoder->start();

  consumer->add_decoder(decoder);

  /*
  * The producer thread only sends JTAG requests
  * The consumer thread read tdo ouput and process result : extract data from
  * jtag protocol
  *
  * A Command object contains a binary array which can be directly send to the
  * device
  * So If a big file has to be send, one command object is give to the Producer
  * that sends it following the USB3 limitation
  */

  // INFO("Command", "Creating RESET command ...");
  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  // producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating Select command ...");
  ap = new AHB_AP();

  arg.push_back(ap->select());

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating IDCODE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating WRITE_U32 command ...");
  arg.push_back(0xFFFFFFFF);
  arg.push_back(0x20000000);
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("Command", "Creating READ_U32 command ...");
  arg.push_back(0x20000000);
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  producer->add_cmd_to_queue(cmd);

  INFO("User", "Press any key to shutdown Avatar");
  while (stopped == 0) {
    std::cin >> stopped;
  }

  INFO("Interface", "Shutting down interfaces ...");
  producer->stop();
  consumer->stop();
  decoder->stop();

  INFO("Device", "Closing device connection ...");
  fx3->quit();

  INFO("main", "Avatar was properly closed");
}
