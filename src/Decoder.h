/**
* @Author: Nassim
* @Date:   2017-03-15T10:58:16+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:49:01+01:00
*/



#ifndef DECODER_H_
#define DECODER_H_

#include "interface/Interface.h"

#include "builder/Command.h"

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
