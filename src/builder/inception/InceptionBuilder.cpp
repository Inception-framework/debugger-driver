/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: InceptionBuilder.cpp
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

#include "InceptionBuilder.h"

#include "../Jtag.h"
#include "../Command.h"

namespace jtag {

InceptionBuilder::InceptionBuilder() {}
InceptionBuilder::~InceptionBuilder() {}

jtag::Command *InceptionBuilder::write(uint32_t address, uint32_t data) {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::WRITE);

  VERBOSE("InceptionBuilder", "Write command at address 0x%08x", address);

  cmd->push_back((uint32_t) 0x14000001);
  cmd->push_back((uint32_t) address);
  cmd->push_back((uint32_t) data);

  // for (auto i = 3; i >= 0; i--)
    // cmd->push_back(address >> (8 * i));

  // for (auto i = 3; i >= 0; i--)
    // cmd->push_back(data >> (8 * i));

  return cmd;
}

jtag::Command *InceptionBuilder::read(uint32_t address) {

  VERBOSE("InceptionBuilder", "Read command at address 0x%08x", address);

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::READ);

  cmd->push_back((uint32_t) 0x24000001); // Read a word of 32bits
  cmd->push_back((uint32_t) address);

  // for (auto i = 3; i >= 0; i--)
    // cmd->push_back((uint32_t) (address >> (8 * i)) );

  cmd->add_tdo(32, 64);

  return cmd;
}

jtag::Command *InceptionBuilder::reset() {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::RESET);

  for(auto i=0; i<2; i++) {
    cmd->push_back((uint32_t)0x30000000);
  }

  return cmd;
}

jtag::Command *InceptionBuilder::idcode() {

  jtag::Command *cmd = new jtag::Command(COMMAND_TYPE::IDCODE);

  cmd->push_back((uint32_t)0x40000000);

  return cmd;
}

jtag::Command *InceptionBuilder::init() {}
}
