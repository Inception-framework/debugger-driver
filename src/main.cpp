/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: main.cpp
    @Last modified by:   Corteggiani
    @Last modified time: 15-Mar-2017
    @License: GPLv3

    Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
    Copyright (C) 2017 Corteggiani Nassim <Corteggiani>

*
*    This program is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by      *
*    the Free Software Foundation, either version 3 of the License, or         *
*    (at your option) any later version.                                       *
*    This program is distributed in the hope that it will be useful,           *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*    GNU General Public License for more details.                              *
*    You should have received a copy of the GNU General Public License         *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
*                                                                              *
*                                                                              *
********************************************************************************/

#include "System.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sys/types.h>

int main(int argc, char *argv[]) {

  System *sys = new System();

  std::stringstream help;
  std::string program_name(argv[0]);
  help << program_name << " (--trace | --benchmark_io | --interractive)";

  if (argc >= 2) {

    for (auto i = 1; i < argc; i++) {
      std::string arg(argv[i]);

      if (arg == "--info")
        INFO("Init", "%s", sys->info().c_str());
      else if (arg == "--debug")
        DEBUG = true;
      else if (arg == "--debug2") {
        DEBUG = true;
        DEBUG2 = true;
      } else if (arg == "--trace") {
        sys->check(TEST_TRACE);

        std::this_thread::sleep_for(std::chrono::seconds(0));

        return 0;
      } else if (arg == "--benchmark_io") {
        sys->check(BENCHMARK_IO);
        return 0;
      } else if (arg == "--flash") {
        sys->check(TEST_FLASH);
        return 0;
      } else if (arg == "--AHB_AP_SCS") {
        sys->check(TEST_AHB_AP_SCS);
        return 0;
      } else if (arg == "--TEST_AHB_AP_CSW") {
        sys->check(TEST_AHB_AP_CSW);
        return 0;
      } else if (arg == "--INTERRUPT_STUB") {
        sys->check(TEST_INTERRUPT_STUB);
        return 0;
      } else if (arg == "--interractive") {
        FILE *pipe = popen("python interactive.py", "r");
        // ALERT("Command Argument", "interractive mode is not yet available");
      }
    }
  }

  sys->stop();

  ALERT("Command Argument", "Wrong argument(s), should be : %s",
        help.str().c_str());

  return 0;
}
