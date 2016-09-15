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
  uint8_t stopped = 0;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  INFO("Device", "Initializing superspeed device ...");
  Device::USBDevice *fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  fx3->init();

  INFO("Interface", "Starting producer thread ...");
  Producer *producer = new Producer(fx3);
  // producer->start();

  INFO("Decoder", "Starting decoder thread ...");
  Decoder *decoder = new Decoder(producer);

  producer->add_decoder(decoder);

  INFO("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  producer->synchrone_process(cmd, &value);

  INFO("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  producer->synchrone_process(cmd, &value);

  INFO("Command", "Creating Select command ...");
  ap = new AHB_AP();

  arg.push_back(ap->select());

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  producer->synchrone_process(cmd, &value);

  INFO("Command", "Creating IDCODE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  producer->synchrone_process(cmd, &value);

  INFO("Command", "Creating 100 000 WRITE_U32 commands ...");

  arg.push_back(0xFFFF00FF);
  arg.push_back(0x20000000);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);
  producer->synchrone_process(cmd, &value);

  printf("    [*] Previous value : 0x%08x \r\n", (uint32_t)value);

  /*
  * Perform 100 000 memory reads through AHB access port and record io access
  * time. At each read value is compared with expected value called oracle.
  * In case of mismatch, the test is aborted and benchmark results displayed.
  */

  uint32_t addr = 0x20000000;
  uint32_t val32 = 0x00000000;

  for (int i = 0; i < 100000 /*65536*/; i++) {

    // WRITE U32
    arg.push_back(val32);
    arg.push_back(addr);
    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::WRITE_U32, arg);

    Benchmark::start();
    producer->synchrone_process(cmd, &value);
    Benchmark::stop();

    // READ U32
    arg.push_back(addr);
    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);

    producer->synchrone_process(cmd, &value);

    Benchmark::inc_nread();

    if ((uint32_t)value != val32) {
      ALERT("ASSERT FAILED", "At test %d returned value differs from oracle "
                             "(0x%08x) : 0x%08x, address 0x%08x",
            i, val32, (uint32_t)value, addr);
      goto stop;
    }

    // addr += 4;
    val32++;
  }
  printf("    [*] Read at 0x20000000 : 0x%08x \r\n", (uint32_t)value);

  // INFO("Command", "Creating 100 000 READ_U32 commands ...");
  // for (int i = 0; i < 1000000; i++) {
  //   // INFO("Command", "Creating WRITE_U32 command ...");
  //   arg.push_back(0x20000000);
  //   cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  //   producer->add_cmd_to_queue(cmd);
  // }

  INFO("User", "Press any key to shutdown Avatar");
  while (stopped == 0) {
    std::cin >> stopped;
  }

stop:

  std::string str = Benchmark::to_string();

  const char *c_str = str.c_str();

  INFO("Benchmark", "%s", c_str);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::EXIT, arg);
  producer->synchrone_process(cmd, &value);

  INFO("Interface", "Shutting down interface Producer ...");
  // producer->stop();

  INFO("Device", "Closing device connection ...");
  fx3->quit();

  delete fx3;
  delete producer;
  delete decoder;
  delete ap;

  INFO("main", "Avatar was properly closed");

  return Benchmark::time();
}
