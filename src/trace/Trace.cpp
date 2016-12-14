#include "Trace.h"

Trace::Trace(Device::USBDevice *dev): stopped(false), device(dev) {}

void Trace::start() {

  this->task = std::thread(&Trace::run, this);
}

void Trace::stop() {

    stopped = true;
}

void Trace::run() {

  while(stopped==false) {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
