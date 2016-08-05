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

      if (cmd->type != EXIT)
        this->process(cmd);
      else
        this->is_running = false;

      this->queue.pop();
    }

    this->unlock();
  }
  printf("\r\n############ Decoder down #################\r\n");
}

bool Decoder::process(jtag::Command *cmd) {

  if (this->decode(cmd, 0))

    delete cmd;

  /*} else if (cmd->type != RESET) {

    cmd->again();

    this->producer->add_cmd_to_queue(cmd);

  }*/
}

bool Decoder::decode(jtag::Command *cmd, uint32_t position) {

  if (cmd->get_tdo()->size() <= position)
    return false;

  uint8_t *data = cmd->get_in_buffer();

  uint32_t begin = cmd->get_tdo()->at(position)->begin;

  uint32_t end = cmd->get_tdo()->at(position)->end - begin;

  uint32_t value = 0;

  if (end == 34)
    value = this->tdo_to_int(&data[begin]);
  else
    ALERT("Decoder", "Unable to decode value of length %dB", end);

  // printf("\r\n[*] Decoding command %s %dB... : 0x%08x \n",
  // cmd->command_name(),
  //        cmd->size(), value);

  if (cmd->get_type() == READ_U32 || cmd->get_type() == WRITE_U32)
    if (!this->check_ack(&data[begin]))
      return false;

  if (cmd->get_tdo()->size() > ++position)
    return this->decode(cmd, position);

  return true;
}

uint32_t Decoder::tdo_to_int(uint8_t *data) {

  uint32_t decoded_value = 0;

  for (unsigned int i = 0; i < 32; i++) {
    // printf("%1x", (data[i] & (1u << 0)));
    decoded_value += (data[i] & (1u << 0)) << i;
  }
  // puts("\r\n");

  return decoded_value;
}

bool Decoder::check_ack(uint8_t *data) {

  if ((data[0] & ((1u << 0))) && (data[1] & (~(1u << 1))) &&
      (data[2] & ((1u << 2))))
    return false;

  return true;
}
