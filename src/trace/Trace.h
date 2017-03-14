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
