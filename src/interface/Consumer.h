/*
 * Consumer.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "../Decoder.h"
#include "Interface.h"

class Consumer : public Interface {

public:
  Consumer(Device::USBDevice *device);

  virtual ~Consumer();

  void start(void);

  void stop(void);

  void add_cmd_to_queue(jtag::Command *cmd);

  void process_jtag_queue(void);

  void add_decoder(Decoder *decoder);

private:
  void notify(jtag::Command *cmd);

  Decoder *decoder;

  std::vector<Decoder *> decoders;
};

#endif /* CONSUMER_H_ */
