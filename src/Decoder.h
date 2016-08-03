/*
 * Producer.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef DECODER_H_
#define DECODER_H_

#include "interface/Interface.h"

#include "jtag/Command.h"

class Producer;

#include <mutex>
#include <queue>
#include <thread>

#define _LOG_ALL
#include "colored.h"

class Decoder : public Interface {

public:
  Decoder(Producer *producer);

  virtual ~Decoder();

  void start();

  void stop();

  /*
  * Thread safe method use by Consumer object to assign TDO outputs to the
  * expected Command object.
  * The Decoder checks JTAG ack and ask the Producer to send the command again
  * if a wait response occurs
  */
  void add_cmd_to_queue(jtag::Command *cmd);

private:
  void process_jtag_queue(void);

  bool decode(jtag::Command *cmd, uint32_t position);

  std::queue<jtag::Command *> queue;

  std::thread task;

  bool is_running;

  Producer *producer;
};

#endif /* DECODER_H_ */
