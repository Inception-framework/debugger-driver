#include "Decoder.h"

#include "interface/Producer.h"
#include "jtag/TDO.h"

Decoder::Decoder(Producer *producer) /*: Interface(NULL)*/ {

  this->producer = producer;
}

Decoder::~Decoder() {}

void Decoder::add_cmd_to_queue(jtag::Command *cmd) {

  this->locker.lock();

  this->queue.push(cmd);

  this->locker.unlock();
}

void Decoder::start() {

  this->is_running = true;

  // this->task = std::thread(&Decoder::process_jtag_queue, this);
}

void Decoder::stop() {

  this->is_running = false;

  // if (this->task.joinable())
  // this->wait();
}

uint64_t Decoder::process_jtag_queue() {

  jtag::Command *cmd = NULL;

  uint64_t answer = 0;

  while (this->is_running) {

    this->locker.lock();

    if (this->queue.empty() == false) {
      this->locker.unlock();

      this->is_running = false;

      cmd = this->queue.front();

      if (cmd->type != EXIT)
        answer = this->process(cmd);
      else
        this->is_running = false;

      this->queue.pop();
    } else
      this->locker.unlock();
  }

  return answer;
}

// void Decoder::process_jtag_queue(void) {
//
//   jtag::Command *cmd = NULL;
//
//   while (this->is_running) {
//
//     this->lock();
//
//     if (this->queue.empty() == false) {
//       this->unlock();
//
//       cmd = this->queue.front();
//
//       if (cmd->type != EXIT)
//         this->process(cmd);
//       else
//         this->is_running = false;
//
//       this->queue.pop();
//     } else
//       this->unlock();
//   }
//   printf("\r\n############ Decoder down #################\r\n");
// }

uint64_t Decoder::process(jtag::Command *cmd) {

  if (this->decode(cmd, 0)) {

    uint64_t answer = cmd->get_answers()[0];

    delete cmd;

    return answer;

  } else if (cmd->type != RESET) {

    cmd->again();

    this->producer->add_cmd_to_queue(cmd);

    return 0;
  }
}

bool Decoder::decode(jtag::Command *cmd, uint32_t position) {

  if (cmd->get_tdo()->size() <= position)
    return false;

  uint8_t *data = cmd->get_in_buffer();

  uint32_t begin = cmd->get_tdo()->at(position)->begin;

  uint32_t end = cmd->get_tdo()->at(position)->end - begin;

  Response *response;

  if (end == 34)
    response = this->tdo_to_int(&data[begin]);
  else
    ALERT("Decoder", "Unable to decode value of length %dB", end);

  // printf("\r\n[*] Decoding command %s %dB... : 0x%08x \n",
  // cmd->command_name(),
  //        cmd->size(), value);32

  if (cmd->get_type() == READ_U32 || cmd->get_type() == WRITE_U32)
    if (!this->check_ack(response->ack))
      return false;

  if (cmd->get_tdo()->size() > ++position)
    return this->decode(cmd, position);
  else {
    cmd->add_answer(response->data_in);
  }

  return true;
}

Response *Decoder::tdo_to_int(uint8_t *data) {

  uint8_t ack;
  uint64_t datain;

  for (unsigned int i = 0; i < 3; i++) {
    // printf("%1x", (data[i] & (1u << 0)));
    ack += (data[i] & (1u << 0)) << i;
  }
  // puts("\r\n");

  for (unsigned int i = 3; i < 35; i++) {
    // printf("%1x", (data[i] & (1u << 0)));
    datain += (data[i] & (1u << 0)) << i;
  }
  // puts("\r\n");

  Response *response = new Response{ack, datain};

  return response;
}

bool Decoder::check_ack(uint8_t ack) {

  if ((ack & ((1u << 0))) && (ack & (~(1u << 1))) && (ack & ((1u << 2))))
    return false;

  return true;
}
