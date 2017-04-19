/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Producer.cpp
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

#include "Producer.h"

#include <stdio.h>

Producer::Producer(Device::USBDevice *new_device) {device = new_device;}

Producer::~Producer() {}

void Producer::synchrone_process(jtag::Command *cmd, uint64_t *value) {

  uint32_t size;
  size = cmd->get_out_buffer_size();

  if (cmd->type != EXIT) {

    #ifdef DEBUG
    VVERBOSE("PRODUCER", "[*] Sending command %s %dB %d...", cmd->command_name(),
             cmd->get_out_buffer_size(), cmd->type);
    #endif

    device->download(cmd->get_out_buffer(), &size);

    //if( cmd->type == READ)
      device->upload(cmd->get_in_buffer(), &size);

    if (cmd->get_type() == READ || cmd->get_type() == IDCODE)
      decoder->process(cmd, value);
  }
}

void Producer::set_decoder(Decoder *new_decoder) {

  decoder = new_decoder;
}
