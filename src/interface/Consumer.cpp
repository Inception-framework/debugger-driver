/*
 * Consumer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Consumer.h"

#include "../Decoder.h"
#include "../device/Device.h"
#include "../jtag/Command.h"

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

      // this->device->upload(cmd->get_in_buffer(), &size);

      this->notify(cmd);

      this->queue.pop();
    }

    this->unlock();
  }

  printf("\r\n############ Consumer down #################\r\n");
}
