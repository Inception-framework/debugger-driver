/*
 * Consumer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Consumer.h"

Consumer::Consumer(Device::USBDevice *device) : Interface(device) {}

Consumer::~Consumer() {}

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

  while (this->is_running) {

    this->lock();

    if (this->queue.empty() == false) {

      cmd = this->queue.front();

      this->device->upload(cmd->get_output_buffer(), cmd->size());

      this->decoder->add_cmd_to_queue(cmd);

      this->queue.pop();

      delete cmd;
    }

    this->unlock();
  }

  printf("\r\n############ Consumer down #################\r\n");
}
