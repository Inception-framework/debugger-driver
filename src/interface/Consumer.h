/*
 * Consumer.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "Interface.h"

#include "../device/Device.h"

#include <thread>

class Consumer : public Interface {

public:

								Consumer(Device::USBDevice* device);

								virtual ~Consumer();

								void start(void);

								void stop(void);

								void add_cmd_to_queue(jtag::Command* cmd);

								void process_jtag_queue (void);
};

#endif /* CONSUMER_H_ */
