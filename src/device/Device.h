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

  void quit();

  /*
   * By sending any byte on channel 2, the device will switch the state machine
   * Switch between JTAG and SWD state machine
   */
  void change_gpif();

  void download(uint8_t *data, uint32_t size);

  void upload(uint8_t *data, uint32_t size);

private:
  bool string_descriptor_equal(libusb_device_handle *device, uint8_t str_index,
                               const char *string);

  void jtag_libusb_open(const uint16_t vid, const uint16_t pid,
                        const char *serial, struct libusb_device_handle **out);

  void jtag_libusb_close();

  void io(uint8_t endpoint);

  libusb_device *dev; /* USBDevice */

  libusb_device_handle *handle; /* Handle */

  struct libusb_context *context;

  unsigned short vid; /* Vendor ID */

  unsigned short pid; /* Product ID */

  unsigned char is_open; /* When device is opened, val = 1 */

  unsigned char busnum; /* The bus number of this device */

  unsigned char devaddr; /* The device address*/

  unsigned char filler; /* Padding to make struct = 16 bytes */

  uint8_t *buffer;

  uint32_t interface;

  uint32_t buffer_limit;

  uint32_t size;

  uint8_t endpoint;

  libusb_device **devs;
};

} /* namespace JTAG */

#endif /* JTAG_H_ */
