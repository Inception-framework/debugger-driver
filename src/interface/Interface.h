/*
 * Interface.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <mutex>
#include <queue>
#include <thread>

#include "../device/Device.h"
#include "../jtag/Command.h"

class Interface {

public:
  Interface(Device::USBDevice *device);

  virtual ~Interface();

  virtual void start(void) = 0;

  virtual void stop(void) = 0;

  virtual void add_cmd_to_queue(jtag::Command *cmd) = 0;

  virtual void process_jtag_queue(void) = 0;

  void lock();

  void unlock();

  void wait();

protected:
  std::queue<jtag::Command *> queue;

  std::thread task;

  bool is_running;

  Device::USBDevice *device;

  std::mutex locker;
};

#endif
