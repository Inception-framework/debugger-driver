/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */
bool DEBUG = false;

#include "main.h"
#include "benchmark/Benchmark.h"
#include "jtag/Command_test.cpp"

#include <string>

#include <iostream>

#include <sstream>

using namespace jtag;

typedef struct {
  Device::USBDevice *fx3;
  Producer *producer;
  Decoder *decoder;
} System;

int trace();

int benchmark_io();

void init(System *system);

void wait_to_close(System *system);

int main(int argc, char *argv[]) {

  std::stringstream info;

  info << "    [*] Avatar gateway version 1.0\r\n";
  info << "            * Author  : Corteggiani Nassim\r\n";
  info << "            * Date    : 2016\r\n";
  info << "            * Company : Maxim Integrated\r\n";
  info << "            * Project : Avatar On Steroids\r\n";

  INFO("Init", "%s", info.str().c_str());

  std::stringstream help;
  std::string program_name(argv[0]);
  help << program_name << " (--trace | --benchmark_io | --interractive)";

  if (argc >= 3) {
    std::string arg(argv[1]);
    std::string arg1(argv[2]);

    if (arg == "--debug" || arg1 == "--debug")
      DEBUG = true;
  }

  if (argc >= 2) {
    std::string arg(argv[1]);

    if (arg == "--trace") {
      return trace();
    } else if (arg == "--benchmark_io") {
      return benchmark_io();
    } else if (arg == "--interractive") {
      ALERT("Command Argument", "interractive mode is not yet available");
    }
  }

  ALERT("Command Argument", "Wrong argument(s), should be : %s",
        help.str().c_str());

  return 0;
}

int trace() {

  SUCCESS("Trace", "Starting Trace Mode ...");

  Command *cmd = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  System system;

  init(&system);

  VERBOSE("Command", "Creating TRACE command ...");

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::TRACE, arg);
  system.producer->synchrone_process(cmd, &value);

  // arg.push_back(0xE0001000);
  // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  // system.producer->synchrone_process(cmd, &value);
  //
  // printf("    [*] Read at 0xE0001000 : 0x%08x \r\n", (uint32_t)value);

  INFO("User", "Press any key to shutdown Avatar");

  wait_to_close(&system);
  return 0;
}

void wait_to_close(System *system) {

  // uint8_t stopped = 0;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  Command *cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::EXIT, arg);
  system->producer->synchrone_process(cmd, &value);

  // INFO("User", "Press any key to shutdown Avatar");
  // while (stopped == 0) {
  //   std::cin >> stopped;
  // }

  INFO("Device", "Closing device connection ...");
  system->fx3->quit();

  INFO("main", "Avatar was properly closed");

  delete system->fx3;
  delete system->producer;
  delete system->decoder;
}

void init(System *system) {

  Command *cmd = NULL;
  AccessPort *ap = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;

  INFO("Device", "Initializing superspeed device ...");
  system->fx3 = new Device::USBDevice(0x04B4, 0x00F0, 0);
  system->fx3->init();

  VERBOSE("Interface", "Starting producer thread ...");
  system->producer = new Producer(system->fx3);

  VERBOSE("Decoder", "Starting decoder thread ...");
  system->decoder = new Decoder(system->producer);

  system->producer->add_decoder(system->decoder);

  VERBOSE("Command", "Creating RESET command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::RESET, arg);
  system->producer->synchrone_process(cmd, &value);

  VERBOSE("Command", "Creating ACTIVE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::ACTIVE, arg);
  system->producer->synchrone_process(cmd, &value);

  VERBOSE("Command", "Creating Select command ...");
  ap = new AHB_AP();

  arg.push_back(ap->select());

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::SELECT, arg);
  system->producer->synchrone_process(cmd, &value);

  VERBOSE("Command", "Creating IDCODE command ...");
  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::IDCODE, arg);
  system->producer->synchrone_process(cmd, &value);
}

int benchmark_io() {

  SUCCESS("Benchmark", "Starting IO Benchmark ...");

  Command *cmd = NULL;
  AccessPort *ap = NULL;
  std::vector<uint32_t> arg;
  uint64_t value = 0;
  bool success = true;
  System system;

  init(&system);

  VERBOSE("Command", "Creating 100 000 WRITE_U32 commands ...");

  // arg.push_back(0xFFFF00FF);
  arg.push_back(0x20000000);

  cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
  system.producer->synchrone_process(cmd, &value);

  printf("    [*] Read at 0x20000000 value : 0x%08x \r\n", (uint32_t)value);

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

    system.producer->synchrone_process(cmd, &value);

    // READ U32
    arg.push_back(addr);
    cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);

    Benchmark::start();
    system.producer->synchrone_process(cmd, &value);
    Benchmark::stop();

    Benchmark::inc_nread();

    if ((uint32_t)value != val32) {
      ALERT("ASSERT FAILED", "At test %d returned value differs from oracle "
                             "(0x%08x) : 0x%08x, address 0x%08x",
            i, val32, (uint32_t)value, addr);

      printf("    [*] Read at 0x%08x : 0x%08x \r\n", (uint32_t)addr,
             (uint32_t)value);
      success = false;
      break;
    }

    // addr += 4;
    val32++;
  }

  if (success)
    INFO("Benchmark", "%s", (const char *)Benchmark::to_string().c_str());

  wait_to_close(&system);

  delete ap;

  return Benchmark::time();
}
