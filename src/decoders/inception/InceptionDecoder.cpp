/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: InceptionDecoder.cpp
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

#include "InceptionDecoder.h"

InceptionDecoder::InceptionDecoder(Producer* new_producer) : Decoder(new_producer) {}

InceptionDecoder::~InceptionDecoder() {}

int32_t InceptionDecoder::process(jtag::Command *cmd, uint64_t *value) {

  *value = 0;

  switch(cmd->get_type()) {

    case WRITE:
    case IDCODE:
    case RESET:
      return 0;
      break;
    case READ:

    uint8_t* buffer = cmd->get_in_buffer();

    if( cmd->get_in_buffer_size() != 8 )  {

      if( cmd->get_in_buffer_size() == 4 ) {

        memcpy(value, &buffer[0], 4);

        ALERT("InceptionDecoder", "Failed to decode READ command with error code 0x%08x", value);
        throw std::runtime_error("Error : wring answer ");
      } else {

        ALERT("InceptionDecoder", "Failed to decode READ command with unknown error code");
        throw std::runtime_error("Error : wring answer ");
      }
    } else {

      // memcpy(value, &buffer[4], 4);
      *value |= buffer[4] << 24;
      *value |= buffer[5] << 16;
      *value |= buffer[6] << 8;
      *value |= buffer[7];

      return (uint32_t)*value;
    }
    break;
  }

  return 0;
}
