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
  // TODO Auto-generated destructor stub
}

uint8_t *Command::get_out_buffer() {
  return (uint8_t *)this->out_buffer.data();
}

uint8_t *Command::get_in_buffer() {

  this->in_buffer.resize(this->out_buffer.size());

  return (uint8_t *)this->in_buffer.data();
}

uint32_t Command::size() { return this->out_buffer.size(); }

void Command::add_command(uint32_t tms, uint32_t tdi, uint32_t trst,
                          uint32_t srst) {

  uint8_t byte = 0;

  byte += (tms << 0);
  byte += (tdi << 1);
  byte += (trst << 2);
  byte += (srst << 3);

  this->out_buffer.push_back(byte);
}

void Command::set_type(COMMAND_TYPE new_type) { this->type = new_type; }

void Command::move_to(tap_state_t state) {

  int i;
  int tms = 0;
  uint8_t byte;

  // printf("From %s to %s ", Jtag::tap_state_name(Jtag::current_state),
  //        Jtag::tap_state_name(state));

  uint8_t tms_scan = Jtag::tap_get_tms_path(Jtag::current_state, state);

  uint8_t tms_scan_bits =
      Jtag::tap_get_tms_path_len(Jtag::current_state, state);

  for (i = 0; i < tms_scan_bits; i++) {

    byte = tms_scan & (1u << i) ? (1 << 0) : 0;
    this->out_buffer.push_back(byte);

    // printf("%01x ", byte);
  }
  // printf("\n\n");

  Jtag::current_state = state;
}

void Command::write_ir(uint8_t ir) {

  uint32_t j;
  uint8_t byte;

  for (j = 0; j < 4; j++) {

    byte = ir & (1u << j) ? (1 << 1) : 0;

    if (j == 3)
      byte += (1 << 0); // set tms

    this->out_buffer.push_back(byte);
  }

  this->out_buffer.push_back(0);

  Jtag::current_state = TAP_IRPAUSE;
}

void Command::wait(uint32_t cycles) {

  switch (Jtag::current_state) {
  case TAP_DREXIT2:
  case TAP_DREXIT1:
  case TAP_DRSHIFT:
  case TAP_DRPAUSE:
  case TAP_DRUPDATE:
  case TAP_DRCAPTURE:
  case TAP_DRSELECT:
    move_to(TAP_DRPAUSE);
    break;

  case TAP_IRSELECT:
  case TAP_IREXIT2:
  case TAP_IREXIT1:
  case TAP_IRSHIFT:
  case TAP_IRPAUSE:
  case TAP_IRUPDATE:
  case TAP_IRCAPTURE:
    move_to(TAP_IRPAUSE);
    break;

  default:
    return;
  }

  for (int i = 0; i < cycles; i++)
    this->out_buffer.push_back(0);
}

void Command::write_dr(uint8_t RnW, uint8_t address, uint32_t datain) {

  uint32_t j;
  uint8_t byte;
  uint32_t pos = this->out_buffer.size();

  // buf_set_u32(&cmd->out_addr_buf, 0, 3, (0x8 >> 1) & 0x6) | (cmd->RnW &
  // 0x1));

  byte = ((address >> 1) & 0x6) | (RnW & 0x1);

  // this->out_buffer.push_back ( RnW == 1 ? (1 << 1) : 0 );

  for (j = 0; j < 3; j++)
    this->out_buffer.push_back(byte & (1u << j) ? (1 << 1) : 0);

  for (j = 0; j < 31; j++)
    this->out_buffer.push_back(datain & (1u << j) ? (1 << 1) : 0);

  this->out_buffer.push_back(datain & (1u << 31) ? (1 << 1) | (1 << 0)
                                                 : (1 << 0));

  this->out_buffer.push_back(0);

  Jtag::current_state = TAP_DRPAUSE;

  tdo.add(pos, (pos + 34));
}

const char *Command::command_name() {

  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(command_name_mapping); i++) {
    if (command_name_mapping[i].symbol == this->type)
      return command_name_mapping[i].name;
  }
  return "???";
}

COMMAND_TYPE Command::get_type() { return this->type; }

int32_t Command::again() { return this->attempts++; }

TDO *Command::get_tdo() { return &tdo; }

} /* namespace JTAG */
