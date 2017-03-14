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

#ifndef JTAG_DECODER_H_
#define JTAG_DECODER_H_

#include "../Decoder.h"

#include "../../builder/TDO.h"

#include <iostream>
#include <stdint.h>
#include <vector>
#include <iomanip>

class JTAGDecoder : public Decoder {

public:

  JTAGDecoder(Producer *new_producer);

  ~JTAGDecoder() {};

  int32_t process(jtag::Command *cmd, uint64_t *value);

private:

  int8_t decode(uint64_t *value);

  int8_t decode_recursivly(jtag::Command *cmd, uint32_t position, uint64_t *value);

  jtag::TDO_PACKET *to_int(jtag::Command *cmd, uint32_t position);

  bool check_ack(uint8_t ack);
};

#endif
