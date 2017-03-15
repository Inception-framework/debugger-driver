/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: TDO.h
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

#ifndef TDO_H_
#define TDO_H_

#include <iostream>
#include <stdint.h>
#include <vector>

namespace jtag {

typedef struct pair {
  uint32_t begin;
  uint32_t end;
} Pair;

typedef struct tdo_packet {
  uint8_t ack;
  uint64_t data_in;
} TDO_PACKET;

class TDO : public std::vector<Pair *> {
public:
  TDO();

  virtual ~TDO();

  void add(uint32_t begin, uint32_t end);

  int8_t decode(uint64_t *value, bool idcode);

  void resize(uint32_t new_size);

  uint8_t *tdo_ptr();

private:
  int8_t decode_recursivly(uint32_t position, uint64_t *value);

  TDO_PACKET *to_int(uint32_t position);

  bool check_ack(uint8_t ack);

  std::vector<uint8_t> data;
};

} /* namespace JTAG */

#endif /* TDO_H_ */
