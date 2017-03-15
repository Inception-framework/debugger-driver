/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Decoder.cpp
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

#include "Decoder.h"

#include "interface/Producer.h"
#include "builder/TDO.h"

Decoder::Decoder(Producer *producer) { this->producer = producer; }

Decoder::~Decoder() {}

void Decoder::add_cmd_to_queue(jtag::Command *cmd) {

  this->locker.lock();

  this->queue.push(cmd);

  this->locker.unlock();
}

int32_t Decoder::process_jtag_queue(uint64_t *value) {

  jtag::Command *cmd = NULL;

  this->locker.lock();
  if (this->queue.empty() == false) {
    this->locker.unlock();

    cmd = this->queue.front();

    this->process(cmd, value);

    this->queue.pop();

    return 0;

  } else
    this->locker.unlock();

  return -1;
}

int32_t Decoder::process(jtag::Command *cmd, uint64_t *value) {

  if (cmd->decode(value) != -1) {

    delete cmd;

    return 0;

  } else {

    if (cmd->again() <= 2)
      this->producer->add_cmd_to_queue(cmd);
    else {

      ALERT("Decoder", "Command %s failed after 2 attempts",
            cmd->command_name());

      delete cmd;
    }

    return -1;
  }
}
