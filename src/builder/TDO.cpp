/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: TDO.cpp
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

#include "TDO.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#define _LOG_ALL
#include "../colored.h"

namespace jtag {

TDO::TDO() {}

TDO::~TDO() {

  for (unsigned int i = 0; i < this->size(); i++)
    delete this->at(i);
}

void TDO::add(uint32_t begin, uint32_t end) {

  this->push_back(new Pair{begin, end});
}

int8_t TDO::decode(uint64_t *value, bool idcode) {

  if (idcode) {

    TDO_PACKET *packet;

    packet = to_int(0);

    *value = (packet->ack) | (packet->data_in << 3);
  } else
    return this->decode_recursivly(0, value);
}

int8_t TDO::decode_recursivly(uint32_t position, uint64_t *value) {

  TDO_PACKET *packet;

  uint32_t begin = this->at(position)->begin;

  uint32_t end = at(position)->end - begin;

  if (this->size() <= position)
    return -1;

  if (end == 34)
    packet = to_int(position);
  else
    return -1;

  if (!this->check_ack(packet->ack))
    return -1;

  if (size() > ++position)
    return decode_recursivly(position, value);
  else
    *value = packet->data_in;
  return 0;
}

TDO_PACKET *TDO::to_int(uint32_t position) {

  uint8_t ack = 0;
  uint64_t datain = 0;

  uint32_t begin = this->at(position)->begin;

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

  TDO_PACKET *packet = new TDO_PACKET{ack, datain};

  return packet;
}

bool TDO::check_ack(uint8_t ack) {

  if ((ack & ((1u << 0))) && (ack & (~(1u << 1))) && (ack & ((1u << 2))))
    return false;

  return true;
}

void TDO::resize(uint32_t new_size) { data.resize(new_size); }

uint8_t *TDO::tdo_ptr() { return (uint8_t *)data.data(); }

} /* namespace JTAG */
