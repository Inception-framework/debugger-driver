/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: CommandsFactory.cpp
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

#include "CommandsFactory.h"

#include "CommandsBuilder.h"

#include "jtag/JTAGBuilder.h"
#include "inception/InceptionBuilder.h"
#include "file_builder/FileBuilder.h"

#include <assert.h>

using namespace std;
using namespace jtag;

CommandsBuilder *CommandsFactory::builder = NULL;

enum JTAG_PROTOCOL CommandsFactory::protocol = JTAG;

CommandsFactory::CommandsFactory() {}

void CommandsFactory::initProtocol(JTAG_PROTOCOL new_protocol) {

  switch (new_protocol) {
  case SWD:
    // builder = new SWDBuilder();
    break;
  case JTAG:
    CommandsFactory::builder = new JTAGBuilder();
    break;
  case INCEPTION:
    CommandsFactory::builder = new InceptionBuilder();
  case FILEB:
    CommandsFactory::builder = new FileBuilder();
    break;
  default:
    throw std::runtime_error(
        "CommandsFactory initialized with an unknown protocol ... \n");
  }

  builder->init();
}

CommandsFactory::~CommandsFactory() {}

jtag::Command *CommandsFactory::CreateCommand(COMMAND_TYPE type,
                                              vector<uint32_t> &argv) {

  assert(CommandsFactory::builder &&
         "You must init a protocol before using the CreateCommand method");

  jtag::Command *cmd = NULL;

  switch (type) {

  case RESET:
    cmd = CommandsFactory::builder->reset();
    break;

  case READ:
    if (check_arg(argv, 1) == true) {
      cmd = CommandsFactory::builder->read(argv.at(0));
    } else
      ALERT("CommandsFactory", "READ args missing");

    break;

  case WRITE:
    if (check_arg(argv, 2) == true) {
      cmd = CommandsFactory::builder->write(argv.at(1), argv.at(0));
    } else
      ALERT("CommandsFactory", "WRITE args missing");

    break;

  case IDCODE:
    cmd = CommandsFactory::builder->idcode();

  case EXIT:
    break;
  }

  argv.clear();

  return cmd;
}

bool CommandsFactory::check_arg(vector<uint32_t> &argv,
                                uint32_t required_size) {

  if (argv.size() != required_size)
    return false;

  return true;
}
