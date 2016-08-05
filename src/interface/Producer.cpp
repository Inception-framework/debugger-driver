/*
 * Producer.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

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

void Producer::process_jtag_queue(void) {

  jtag::Command *cmd = NULL;

  uint32_t size;

  while (this->is_running) {

    this->lock();

    if (this->queue.empty() == false) {
      this->unlock();

      cmd = this->queue.front();
      //
      // printf("\r\n[*] Sending command %s %dB...\n", cmd->command_name(),
      //        cmd->size());
      // for (unsigned int i = 0; i < cmd->size(); i++)
      //   printf("%02x", cmd->get_buffer()[i]);
      // printf("\r\n");

      size = cmd->size();

      if (cmd->type != EXIT) {

        this->device->download(cmd->get_out_buffer(), &size);

        this->device->upload(cmd->get_in_buffer(), &size);

      } else
        this->is_running = false;

      this->notify(cmd);

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

  for (it = decoders.begin(); it != decoders.end(); ++it) {

    (*it)->add_cmd_to_queue(cmd);
  }
}
