/**
* @Author: Nassim
* @Date:   2016-12-14T17:22:42+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T11:49:54+01:00
*/



/*
 * Device.h
 *
 *  Created on: Jul 12, 2016
 *      Author: noname
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <libusb-1.0/libusb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _LOG_ALL
#include "../colored.h"

#include <exception>
#include <stdexcept>

namespace Device {

class USBDevice {
public:
  USBDevice(uint16_t vid, uint16_t pid, uint32_t interface);

  virtual ~USBDevice();

  void init();

  void close();

  void download(uint8_t *data, uint32_t *size);

  void upload(uint8_t *data, uint32_t *size);

private:

  void device_open();

  void device_close();

  uint32_t io(uint8_t endpoint, uint8_t *buffer, uint32_t size);

  libusb_device *dev; /* USBDevice */

  libusb_device_handle *handle; /* Handle */

  struct libusb_context *context;

  struct libusb_device_descriptor descriptor;

  unsigned short vid; /* Vendor ID */

  unsigned short pid; /* Product ID */

  unsigned char is_open; /* When device is opened, val = 1 */

  unsigned char busnum; /* The bus number of this device */

  unsigned char devaddr; /* The device address*/

  unsigned char filler; /* Padding to make struct = 16 bytes */

  uint32_t interface;

  uint32_t buffer_limit;

  uint8_t endpoint;
};

} /* namespace JTAG */

#endif /* JTAG_H_ */
