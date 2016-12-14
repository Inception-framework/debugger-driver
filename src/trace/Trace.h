#ifndef TRACE_H
#define TRACE_H

#include <thread>
#include <chrono>

#include "../device/Device.h"

class Trace {

public:

  Trace(Device::USBDevice *device);

  ~Trace();

  void start();

  void stop();

private:

  void run();

  std::thread task;

  bool stopped;

  Device::USBDevice* device;

};

#endif
