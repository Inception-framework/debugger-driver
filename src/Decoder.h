/*
 * Decoder.h
 *
 *  Created on: Jul 12, 2016
 *      Author: Nassim Corteggiani
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

class Decoder /*: public Interface*/ {

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

  int32_t process_jtag_queue(uint64_t*);

  int32_t process(jtag::Command *cmd, uint64_t *value);
private:

  bool check_ack(uint8_t data);

  std::queue<jtag::Command *> queue;

  std::thread task;

  bool is_running;

  Producer *producer;

  std::mutex locker;
};

#endif /* DECODER_H_ */
