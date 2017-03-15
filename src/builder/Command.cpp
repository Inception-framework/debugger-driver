/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Command.cpp
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

#include "Command.h"

namespace jtag {

Command::Command(COMMAND_TYPE type) {

  this->type = type;

  this->attempts = 0;
}

Command::~Command() {

  // delete &out_buffer;
  // delete &in_buffer;
  // delete &tdo;
}

uint8_t *Command::get_out_buffer() {

  return (uint8_t *)this->out_buffer.data();
}

uint8_t *Command::get_in_buffer() {

  tdo.resize(out_buffer.size());

  return (uint8_t *)tdo.tdo_ptr();
}

uint32_t Command::size() { return this->out_buffer.size(); }

void Command::set_type(COMMAND_TYPE new_type) { this->type = new_type; }

const char *Command::command_name() {

  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(command_name_mapping); i++) {
    if (command_name_mapping[i].symbol == this->type)
      return command_name_mapping[i].name;
  }
  return "???";
}

COMMAND_TYPE Command::get_type() { return this->type; }

uint8_t Command::again() { return ++this->attempts; }

int8_t Command::decode(uint64_t *value) {

  if (type == READ || type == IDCODE)
    return tdo.decode(value, type == IDCODE ? true : false);
  else
    return 0;
}

void Command::push_back(uint8_t data_in) { out_buffer.push_back(data_in); }

void Command::add_tdo(uint64_t start, uint64_t end) { tdo.add(start, end); }

} /* namespace JTAG */
