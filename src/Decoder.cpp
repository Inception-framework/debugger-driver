#include "Decoder.h"

#include "interface/Producer.h"

Decoder::Decoder(Producer *producer) : Interface(NULL) {

  this->producer = producer;
}

Decoder::~Decoder() {}

void Decoder::add_cmd_to_queue(jtag::Command *cmd) {

  this->lock();

  this->queue.push(cmd);

  this->unlock();
}

void Decoder::start() {

  this->is_running = true;

  this->task = std::thread(&Decoder::process_jtag_queue, this);
}

void Decoder::stop() {

  this->is_running = false;

  this->wait();
}

void Decoder::process_jtag_queue(void) {

  jtag::Command *cmd = NULL;

  while (this->is_running) {

    this->lock();

    if (this->queue.empty() == false) {

      cmd = this->queue.front();

      printf("\r\n[*] Decoding command %s %dB...\n", cmd->command_name(),
             cmd->size());
      for (unsigned int i = 0; i < cmd->size(); i++)
        printf("%02x", cmd->get_in_buffer()[i]);
      printf("\r\n");

      if (true /*this->check_ack(cmd)*/) {

        this->queue.pop();

        delete cmd;
      } else {

        cmd->again();

        this->producer->add_cmd_to_queue(cmd);
      }
    }

    this->unlock();
  }

  printf("\r\n############ Decoder down #################\r\n");
}
