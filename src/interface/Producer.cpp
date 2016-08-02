/*
 * Producer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#include "Producer.h"

int Producer::sent = 0;

Producer::Producer(Device::USBDevice *device, Consumer *consumer)
    : Interface(device) {

  this->consumer = consumer;
}

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

  this->wait();
}

void Producer::process_jtag_queue(void) {

  jtag::Command *cmd = NULL;

  while (this->is_running) {

    this->lock();

    /* The producer goal is to use the maximum of the USB3 bandwidth */
    if (this->queue.empty() == false) {

      cmd = this->queue.front();

      printf("\r\n[*] Sending command %s %dB...\n", cmd->command_name(),
             cmd->size());
      // for (unsigned int i = 0; i < cmd->size(); i++)
      //   printf("%02x", cmd->get_buffer()[i]);
      // printf("\r\n");

      this->device->download(cmd->get_out_buffer(), cmd->size());

      this->queue.pop();

      // delete cmd;

      this->consumer->add_cmd_to_queue(cmd);

      Producer::sent++;
    }

    this->unlock();
  }

  printf("\r\n############ Producer down #################\r\n");
}
