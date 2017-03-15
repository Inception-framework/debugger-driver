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

int Producer::sent = 0;

Producer::Producer(Device::USBDevice *device) : Interface(device) {}

Producer::~Producer() {}

void Producer::add_cmd_to_queue(jtag::Command *cmd) {

  this->lock();

  this->queue.push(cmd);

  this->unlock();
}

void Producer::start() {

  this->is_running = true;

  this->task = std::thread(&Producer::process_jtag_queue, this);
}

void Producer::stop() {

  this->is_running = false;

  // if (this->task.joinable())
  this->wait();
}

void Producer::synchrone_process(jtag::Command *cmd, uint64_t *value) {

  uint32_t size;

  size = cmd->size();

  if (cmd->type != EXIT) {

    VVERBOSE("PRODUCER", "[*] Sending command %s %dB...", cmd->command_name(),
             cmd->size());

    this->device->download(cmd->get_out_buffer(), &size);

    this->device->upload(cmd->get_in_buffer(), &size);

    if (cmd->type == READ || cmd->type == IDCODE)
      this->decoders.at(0)->process(cmd, value);
  }
}

void Producer::process_jtag_queue(void) {

  jtag::Command *cmd = NULL;

  uint32_t size;

  while (this->is_running) {

    this->lock();

    if (this->queue.empty() == false) {
      this->unlock();

      cmd = this->queue.front();

      VVERBOSE("PRODUCER", "[*] Sending command %s %dB...", cmd->command_name(),
               cmd->size());

      size = cmd->size();

      if (cmd->type != EXIT) {

        this->device->download(cmd->get_out_buffer(), &size);

        this->device->upload(cmd->get_in_buffer(), &size);

        this->notify(cmd);

      } else
        this->is_running = false;

      this->queue.pop();

      Producer::sent++;
    } else
      this->unlock();
  }

  printf("\r\n############ Producer down #################\r\n");
}

void Producer::add_decoder(Decoder *decoder) {

  this->decoders.push_back(decoder);
}

void Producer::notify(jtag::Command *cmd) {

  std::vector<Decoder *>::iterator it;

  if (cmd->type != READ)
    return;

  for (it = decoders.begin(); it != decoders.end(); ++it) {

    (*it)->add_cmd_to_queue(cmd);
  }
}
