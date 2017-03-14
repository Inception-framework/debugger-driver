/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: JTAGDecoder.h
    @Last modified by:   Corteggiani
    @Last modified time: 12-Apr-2017
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

#include "JTAGDecoder.h"

using namespace jtag;

JTAGDecoder::JTAGDecoder(Producer *new_producer) : Decoder(new_producer) {}

int32_t JTAGDecoder::process(jtag::Command *cmd, uint64_t *value) {

  if (cmd->get_type() == IDCODE) {

    TDO_PACKET *packet;

    packet = to_int(cmd, 0);

    *value = (packet->ack) | (packet->data_in << 3);
  } else
    return decode_recursivly(cmd, 0, value);
}

int8_t JTAGDecoder::decode_recursivly(jtag::Command *cmd, uint32_t position, uint64_t *value) {

  TDO_PACKET *packet;

  uint32_t begin = cmd->tdo.at(position)->begin;

  uint32_t end = cmd->tdo.at(position)->end - begin;

  if (cmd->tdo.size() <= position)
    return -1;

  if (end == 34)
    packet = to_int(cmd, position);
  else
    return -1;

  if (!check_ack(packet->ack))
    return -1;

  if (cmd->tdo.size() > ++position)
    return decode_recursivly(cmd, position, value);
  else
    *value = packet->data_in;
  return 0;
}

jtag::TDO_PACKET *JTAGDecoder::to_int(jtag::Command *cmd, uint32_t position) {

  uint8_t ack = 0;
  uint64_t datain = 0;

  uint32_t begin = cmd->tdo.at(position)->begin;

  uint8_t* data = cmd->get_in_buffer();

  std::stringstream info;

  info << "Ack ";
  for (unsigned int i = 0; i < 3; i++) {
    info << std::hex << std::setw(1) << (data[begin + i] & (1u << 0));
    ack += (data[begin + i] & (1u << 0)) << i;
  }
  info << "\r\n";

  info << "\tDATA_IN : ";
  for (unsigned int i = 3; i < 35; i++) {
    info << std::hex << std::setw(1) << (data[begin + i] & (1u << 0));
    datain += (data[begin + i] & (1u << 0)) << i - 3;
  }
  info << "\r\n";

  VVERBOSE("TDO", "%s", info.str().c_str());

  jtag::TDO_PACKET *packet = new jtag::TDO_PACKET{ack, datain};

  return packet;
}

bool JTAGDecoder::check_ack(uint8_t ack) {

  if ((ack & ((1u << 0))) && (ack & (~(1u << 1))) && (ack & ((1u << 2))))
    return false;

  return true;
}
