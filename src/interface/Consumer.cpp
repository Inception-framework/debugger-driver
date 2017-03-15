/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Consumer.cpp
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

#include "Consumer.h"

#include "../Decoder.h"
#include "../device/Device.h"
#include "../builder/Command.h"

Consumer::Consumer(Device::USBDevice *device) : Interface(device) {

  this->decoder = decoder;
}

Consumer::~Consumer() {}

void Consumer::add_decoder(Decoder *decoder) {

  this->decoders.push_back(decoder);
}

void Consumer::notify(jtag::Command *cmd) {

  std::vector<Decoder *>::iterator it;

  for (it = decoders.begin(); it != decoders.end(); ++it) {

    (*it)->add_cmd_to_queue(cmd);
  }
}

void Consumer::add_cmd_to_queue(jtag::Command *cmd) {

  this->lock();

  this->queue.push(cmd);

  this->unlock();
}

void Consumer::start() {

  this->is_running = true;

  this->task = std::thread(&Consumer::process_jtag_queue, this);
}

void Consumer::stop() {

  this->is_running = false;

  this->wait();
}

void Consumer::process_jtag_queue(void) {

  jtag::Command *cmd = NULL;

  uint32_t size;

  while (this->is_running) {

    this->lock();

    if (this->queue.empty() == false) {

      cmd = this->queue.front();

      size = cmd->size();

      if (cmd->type != EXIT)
        this->device->upload(cmd->get_in_buffer(), &size);
      else
        break;

      this->notify(cmd);

      // if (cmd->type == EXIT)
      //   this->is_running = false;

      this->queue.pop();
    }

    this->unlock();
  }

  printf("\r\n############ Consumer down #################\r\n");
}
