/**
* @Author: Nassim
* @Date:   2016-09-15T10:10:04+02:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:48:10+01:00
*/



#include "Interface.h"

Interface::Interface(Device::USBDevice *device) {

  this->device = device;

  this->is_running = false;
}

Interface::~Interface() {
  // TODO Auto-generated destructor stub
}

void Interface::lock() { this->locker.lock(); }

void Interface::unlock() { this->locker.unlock(); }

void Interface::wait() { this->task.join(); }
