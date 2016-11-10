/*
 * main.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: Corteggiani Nassim
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
      sys->check(TEST_TRACE);
      return 0;
    } else if (arg == "--benchmark_io") {
      sys->check(BENCHMARK_IO);
      return 0;
    } else if (arg == "--idcode") {
      sys->check(TEST_IDCODE);
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
