/**
* @Author: Nassim
* @Date:   2016-09-23T11:03:22+02:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:26+01:00
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

  void synchrone_process(jtag::Command *cmd, uint64_t *value);

private:
  void notify(jtag::Command *cmd);

  std::vector<Decoder *> decoders;
};

#endif /* PRODUCER_H_ */
