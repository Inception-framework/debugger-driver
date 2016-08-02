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
        printf("%1x", cmd->get_in_buffer()[i] & (1u << 0));
      printf("\r\n");

      if (this->check(cmd)) {

        delete cmd;
      } else {

        cmd->again();

        this->producer->add_cmd_to_queue(cmd);
      }
      this->queue.pop();
    }

    this->unlock();
  }

  printf("\r\n############ Decoder down #################\r\n");
}

bool Decoder::check(jtag::Command *cmd) {

  uint8_t *data = cmd->get_in_buffer();

  if ((data[0] & ((1u << 0))) && (data[0] & (~(1u << 1))) &&
      (data[0] & ((1u << 2))))
    return true;
  return false;
}
