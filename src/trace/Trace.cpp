/**
* @Author: Nassim
* @Date:   2016-12-16T19:11:27+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:42+01:00
*/



#include "Trace.h"

Trace::Trace(Device::USBDevice *dev): stopped(false), device(dev) {}

void Trace::start() {

  std::async(&Trace::run, this);

}

void Trace::stop() {

    stopped = true;

    // task.get();
}

void Trace::run() {

  uint32_t size = 8;

  uint8_t buffer[8] = {0};

  // buffer[0] = 4;
  // device->download((uint8_t*)&buffer, &size);
  // buffer[0] = 0;

  while(true) {

    INFO("Trace", "Waiting Trace information");

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    device->upload((uint8_t*)&buffer, &size);

    INFO("TRACE", "Received %d B", size);

    for(auto i=0; i<size; i++) {
      printf("%d", buffer[i]);
    }
    printf("\n\r");

  }
}
