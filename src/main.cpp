/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */
#include "main.h"
#include "benchmark/Benchmark.h"
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
  // uint8_t stopped = 0;
  std::vector<uint32_t> arg;
  uint64_t value;

  INFO("Device", "Initializing superspeed device ...");
  Device::USBDevice *fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  INFO("Interface", "Starting producer thread ...");
  Producer *producer = new Producer(fx3);
  producer->start();

  INFO("Decoder", "Starting decoder thread ...");
  Decoder *decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  INFO("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  producer->add_cmd_to_queue(cmd);

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

  INFO("Command", "Creating 100 000 WRITE_U32 commands ...");
  for (int i = 0; i < 100000; i++) {

    arg.push_back(0xffffffff);
    arg.push_back(0x20000000);
    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);
    producer->add_cmd_to_queue(cmd);
    decoder->process_jtag_queue(&value);

    arg.push_back(0x20000000);
    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
    producer->add_cmd_to_queue(cmd);
    decoder->process_jtag_queue(&value);
  }

  // INFO("Command", "Creating 100 000 READ_U32 commands ...");
  // for (int i = 0; i < 1000000; i++) {
  //   // INFO("Command", "Creating WRITE_U32 command ...");
  //   arg.push_back(0x20000000);
  //   cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  //   producer->add_cmd_to_queue(cmd);
  // }

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::EXIT, arg);
  producer->add_cmd_to_queue(cmd);

  // INFO("User", "Press any key to shutdown Avatar");
  // while (stopped == 0) {
  //   std::cin >> stopped;
  // }

  INFO("Interface", "Shutting down interface Producer ...");
  producer->stop();

  INFO("Device", "Closing device connection ...");
  fx3->quit();

  delete fx3;
  delete producer;
  delete decoder;
  delete ap;

  INFO("main", "Avatar was properly closed");
}
