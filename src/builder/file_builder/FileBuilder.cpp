/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: FileBuilder.cpp
    @Last modified by:   Corteggiani
    @Last modified time: 21-Mar-2017
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

#include "FileBuilder.h"

#include "../Jtag.h"
#include "../Command.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <exception>
#include <stdexcept>

using namespace std;

namespace jtag {

FileBuilder::FileBuilder() {}

FileBuilder::~FileBuilder() {}

jtag::Command *FileBuilder::reset() {

    INFO("FileBuilder", "Resetting JTAG debugger");

    jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::RESET);

    // for (int i = 0; i < 5; i++)
      // add_command(cmd, 1, 0, 0, 0);

    // active(cmd);
  //
    // select(cmd, 0);

    process(cmd);

    return cmd;
}

jtag::Command *FileBuilder::init() {
  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::RESET);
  return cmd;
}

void FileBuilder::process(jtag::Command* cmd) {

  std::ifstream is ("io/output.txt");

  if (is) {
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char [length];

    INFO("FileBuilder", "Reading input file of %d Bytes-length ", length);

    // read data as a block:
    is.read (buffer,length);

    if (is)
      INFO("FileBuilder", "All characters read successfully");
    else {
      ALERT("FileBuilder", "Only %d Bytes could be read of %d", is.gcount(), length);
      throw std::runtime_error(
        "A driver different than avatar driver has claimed the interface\n");
    }

    process_inputs(buffer, length, cmd);

    is.close();

    delete[] buffer;
  }else
  {
    ALERT("FileBuilder", "Input.txt file not found");
    throw std::runtime_error(
        "Input.txt file not found\n");
  }
}

void FileBuilder::add_command(jtag::Command *cmd, uint32_t tms, uint32_t tdi,
                              uint32_t trst, uint32_t srst) {

  uint8_t byte = 0;

  byte += (tms << 0);
  byte += (tdi << 1);
  byte += (trst << 2);
  byte += (srst << 3);

  cmd->push_back(byte);
}

void FileBuilder::process_inputs(char* buffer, unsigned size, jtag::Command* cmd) {

  unsigned tms = 0;
  unsigned tdi = 0;

  INFO("FileBuilder", "Buffer %s", buffer);
  INFO("FileBuilder","");

  for(auto i=0; i<size ;i++) {
    // INFO("FileBuilder", "Buffer[%d]=%c", i, buffer[i]);

    switch(buffer[i]) {
      default :
        printf("TMS: %d       TDI: %d\n", tms, tdi);
        add_command(cmd, tms, tdi, 0, 0);
        continue;
      case '0':
        if(i==0 || (i%3==0))
          tms = 0;
        else
          tdi = 0;
      break;
      case '1':
        if(i==0 || (i%3==0))
          tms = 1;
        else
          tdi = 1;
      break;
    }
  }

}


/*
* Implementation of Commands from the CommandsBuilder interface
*/
jtag::Command *FileBuilder::write(uint32_t address, uint32_t datain) {

  ALERT("FileBuilder", "Write command is not available");

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);
  return cmd;
}

jtag::Command *FileBuilder::read(uint32_t address) {

  ALERT("FileBuilder", "Read command is not available");

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);
  return cmd;
}

jtag::Command *FileBuilder::idcode() {

  ALERT("FileBuilder", "IDCode command is not available");

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);
  return cmd;
}

}
