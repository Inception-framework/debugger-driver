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

#include "builder/Command.h"

namespace jtag {

Command::Command(COMMAND_TYPE new_type) {

  type = new_type;

  attempts = 0;
}

Command::~Command() {}

uint8_t *Command::get_out_buffer() {

  return (uint8_t *)out_buffer.data();
}

uint32_t Command::get_out_buffer_size() {

  return out_buffer.size();
}

uint8_t *Command::get_in_buffer() {

  in_buffer.resize(out_buffer.size());

  return (uint8_t *)in_buffer.data();
}

uint32_t Command::get_in_buffer_size() {

  return in_buffer.size();
}


void Command::set_type(COMMAND_TYPE new_type) { type = new_type; }

const char *Command::command_name() {

  for (auto i = 0; i < ARRAY_SIZE(command_name_mapping); i++) {
    if (command_name_mapping[i].symbol == type)
      return command_name_mapping[i].name;
  }
  return "???";
}

COMMAND_TYPE Command::get_type() { return type; }

uint8_t Command::again() { return ++attempts; }

void Command::push_back(uint8_t data_in) {
  out_buffer.push_back(data_in);
}

void Command::push_back(uint32_t data_in) {

  uint8_t* data = (uint8_t *)out_buffer.data();

  // uint32_t size = out_buffer.size();
  out_buffer.push_back(data_in >> 24);
  out_buffer.push_back(data_in >> 16);
  out_buffer.push_back(data_in >> 8);
  out_buffer.push_back(data_in & 0xFF);

  // memcpy((void*)&data[size], (void*)&data_in, 4);
}

void Command::add_tdo(uint64_t start, uint64_t end) { tdo.add(start, end); }

} /* namespace JTAG */
