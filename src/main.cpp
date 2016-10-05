/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */
bool DEBUG = false;

#include "System.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/types.h>

int main(int argc, char *argv[]) {

  System *sys = new System();

  INFO("Init", "%s", sys->info().c_str());

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
      return 0;
    } else if (arg == "--benchmark_io") {
      sys->check(BENCHMARK_IO);
      return 0;
    } else if (arg == "--interractive") {
      FILE *pipe = popen("python interactive.py", "r");
      // ALERT("Command Argument", "interractive mode is not yet available");
    }
  }

  ALERT("Command Argument", "Wrong argument(s), should be : %s",
        help.str().c_str());

  return 0;
}

// int trace() {
//
//   SUCCESS("Trace", "Starting Trace Mode ...");
//
//   Command *cmd = NULL;
//   std::vector<uint32_t> arg;
//   uint64_t value = 0;
//
//   System system;
//
//   init(&system);
//
//   VERBOSE("Command", "Creating TRACE command ...");
//
//   cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::TRACE, arg);
//   system.producer->synchrone_process(cmd, &value);
//
//   // arg.push_back(0xE0001000);
//   // cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::READ_U32, arg);
//   // system.producer->synchrone_process(cmd, &value);
//   //
//   // printf("    [*] Read at 0xE0001000 : 0x%08x \r\n", (uint32_t)value);
//
//   INFO("User", "Press any key to shutdown Avatar");
//
//   wait_to_close(&system);
//   return 0;
// }
//
// void wait_to_close(System *system) {
//
//   // uint8_t stopped = 0;
//   std::vector<uint32_t> arg;
//   uint64_t value = 0;
//
//   Command *cmd = CommandsFactory::CreateCommand(COMMAND_TYPE::EXIT, arg);
//   system->producer->synchrone_process(cmd, &value);
//
//   // INFO("User", "Press any key to shutdown Avatar");
//   // while (stopped == 0) {
//   //   std::cin >> stopped;
//   // }
//
//   INFO("Device", "Closing device connection ...");
//   system->fx3->quit();
//
//   INFO("main", "Avatar was properly closed");
//
//   delete system->fx3;
//   delete system->producer;
//   delete system->decoder;
// }
