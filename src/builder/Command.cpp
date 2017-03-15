/*
 * Command.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

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

void Command::push_back(uint8_t data_in) {

  out_buffer.push_back(data_in);
}

void Command::add_tdo(uint64_t start, uint64_t end) {

  tdo.add(start, end);
}


} /* namespace JTAG */
