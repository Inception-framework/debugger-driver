#include "Decoder.h"

#include "interface/Producer.h"
#include "jtag/TDO.h"

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

      if (this->decode(cmd, 0)) {
        delete cmd;
      } else if (cmd->type != RESET) {

        cmd->again();

        this->producer->add_cmd_to_queue(cmd);
      }

      this->queue.pop();
    }

    this->unlock();
  }

  printf("\r\n############ Decoder down #################\r\n");
}

bool Decoder::decode(jtag::Command *cmd, uint32_t position) {

  uint32_t begin, end;

  uint8_t *data = cmd->get_in_buffer();

  if (cmd->get_tdo()->size() <= position)
    return false;

  begin = cmd->get_tdo()->at(position)->begin;
  end = cmd->get_tdo()->at(position)->end;

  for (unsigned int i = begin; i <= end; i++)
    printf("%1x", data[i] & (1u << 0));
  printf("\r\n");

  if ((data[begin] & ((1u << 0))) && (data[begin + 1] & (~(1u << 1))) &&
      (data[begin + 2] & ((1u << 2))))
    return false;

  if (cmd->get_tdo()->size() > position + 1)
    return this->decode(cmd, position + 1);
  else
    return true;
}
