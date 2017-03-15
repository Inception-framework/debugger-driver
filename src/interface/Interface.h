/**
* @Author: Nassim
* @Date:   2017-03-15T10:57:51+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:14+01:00
*/



#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <mutex>
#include <queue>
#include <thread>

#include "../device/Device.h"
#include "../builder/Command.h"

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
