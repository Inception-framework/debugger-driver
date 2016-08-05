/*
 * Producer.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef PRODUCER_H_
#define PRODUCER_H_

#include "Interface.h"

#include "../device/Device.h"

#include "../Decoder.h"

#include <chrono>
#include <thread>

class Producer : public Interface {

public:
  Producer(Device::USBDevice *device);

  virtual ~Producer();

  void start(void);

  void stop(void);

  void add_cmd_to_queue(jtag::Command *cmd);

  void process_jtag_queue(void);

  static int sent;

  void add_decoder(Decoder *decoder);

private:
  void notify(jtag::Command *cmd);

  std::vector<Decoder *> decoders;
};

#endif /* PRODUCER_H_ */
