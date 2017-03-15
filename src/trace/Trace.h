/**
* @Author: Nassim
* @Date:   2016-12-15T08:59:42+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:43+01:00
*/



#ifndef TRACE_H
#define TRACE_H

#include <chrono>
#include <future>

#include "../device/Device.h"

class Trace {

public:

  Trace(Device::USBDevice *device);

  ~Trace();

  void start();

  void stop();

  void run();

private:

  // std::future task;

  bool stopped;

  Device::USBDevice* device;

};

#endif
