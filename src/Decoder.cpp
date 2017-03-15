/**
* @Author: Nassim
* @Date:   2017-03-15T11:33:28+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:47:57+01:00
*/



#include "Decoder.h"

#include "interface/Producer.h"
#include "builder/TDO.h"

Decoder::Decoder(Producer *producer) { this->producer = producer; }

Decoder::~Decoder() {}

void Decoder::add_cmd_to_queue(jtag::Command *cmd) {

  this->locker.lock();

  this->queue.push(cmd);

  this->locker.unlock();
}

int32_t Decoder::process_jtag_queue(uint64_t *value) {

  jtag::Command *cmd = NULL;

  this->locker.lock();
  if (this->queue.empty() == false) {
    this->locker.unlock();

    cmd = this->queue.front();

    this->process(cmd, value);

    this->queue.pop();

    return 0;

  } else
    this->locker.unlock();

  return -1;
}

int32_t Decoder::process(jtag::Command *cmd, uint64_t *value) {

  if (cmd->decode(value) != -1) {

    delete cmd;

    return 0;

  } else {

    if (cmd->again() <= 2)
      this->producer->add_cmd_to_queue(cmd);
    else {

      ALERT("Decoder", "Command %s failed after 2 attempts",
            cmd->command_name());

      delete cmd;
    }

    return -1;
  }
}
