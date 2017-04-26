/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Device.h
    @Last modified by:   noname
    @Last modified time: 16-Mar-2017
    @License: GPLv3

    Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
    Copyright (C) 2017 Corteggiani Nassim <Corteggiani>

*
*    This program is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by      *
*    the Free Software Foundation, either version 3 of the License, or         *
*    (at your option) any later version.                                       *
*    This program is distributed in the hope that it will be useful,           *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*    GNU General Public License for more details.                              *
*    You should have received a copy of the GNU General Public License         *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
*                                                                              *
*                                                                              *
********************************************************************************/



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
  USBDevice(uint16_t vid, uint16_t pid, uint32_t interface,  uint8_t out = 0x01, uint8_t in=0x81);

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

  uint8_t entrypoint_download;

  uint8_t entrypoint_upload;
};

} /* namespace JTAG */

#endif /* JTAG_H_ */
