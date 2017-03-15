/**
* @Author: Nassim
* @Date:   2017-03-15T10:58:57+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:07+01:00
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
