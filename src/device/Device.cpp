/*******************************************************************************
    @Author: Corteggiani Nassim <Corteggiani>
    @Email:  nassim.corteggiani@maximintegrated.com
    @Filename: Device.cpp
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

#include "Device.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace Device {

libusb_device_handle* USBDevice::handle = NULL;

bool USBDevice::initialized = false;

USBDevice::USBDevice(uint16_t p_vid, uint16_t p_pid, uint32_t p_interface, uint8_t out, uint8_t in, unsigned int timeout) {

  this->timeout=timeout;

  entrypoint_download = out;
  entrypoint_upload = in;

  vid = p_vid;

  pid = p_pid;

  interface = p_interface;

  buffer_limit = 1024;
}

USBDevice::~USBDevice() {
  // TODO Auto-generated destructor stub
}

void USBDevice::close(void) { device_close(); }

void USBDevice::device_open() {

  int cnt, idx, errCode;
  libusb_device **devs;

  if (libusb_init(&context) < 0)
    return;

  cnt = libusb_get_device_list(context, &devs);

  for (idx = 0; idx < cnt; idx++) {

    VVERBOSE("Device",
             "Detecting device idVendor = %02x (Expected idProduct = %02x)",
             descriptor.idVendor, descriptor.idProduct);

    if (libusb_get_device_descriptor(devs[idx], &descriptor) != 0)
      continue;

    if (descriptor.idVendor != vid || descriptor.idProduct != pid) {
      continue;
    }

    dev = devs[idx];

    errCode = libusb_open(devs[idx], &USBDevice::handle);

    if (errCode) {
      USBDevice::handle = NULL;
      ALERT("Device", "libusb_open() failed with %s",
            libusb_error_name(errCode));
      continue;
    }

    break;
  }
  if (cnt >= 0)
    libusb_free_device_list(devs, 1);
}

void USBDevice::device_close() {
  /* Close device */
  libusb_close(USBDevice::handle);

  libusb_exit(context);
}

void USBDevice::init(void) {

  if(USBDevice::initialized)
    return;

  USBDevice::initialized = true;

  int32_t retval;

  device_open();

  if (!USBDevice::handle) {
    ALERT("Device", "Avatar driver doesn't find device %04x:%04x \n", vid, pid);
    throw std::runtime_error("Avatar driver doesn't find device\n");
    return;
  }

  dev = libusb_get_device(USBDevice::handle);

  if (vid != descriptor.idVendor) {

    ALERT("DEVICE", " ID Vendor %04x:%04x", vid, descriptor.idVendor);
    exit(0);
  }

  if (pid != descriptor.idProduct) {

    ALERT("DEVICE", " ID Product %04x:%04x", pid, descriptor.idProduct);
    exit(0);
  }

  is_open = 1;

  busnum = libusb_get_bus_number(dev);
  devaddr = libusb_get_device_address(dev);

  retval = libusb_claim_interface(USBDevice::handle, interface);
  if (retval == 0) {
    INFO("Device", "Avatar USB3 driver successfully claimed interface");
  } else {

    ALERT("ALERT", "Device %04x:%04x", vid, pid);

    switch (retval) {
    case LIBUSB_ERROR_NOT_FOUND:
      ALERT("Device",
            "The interface requested by avatar driver does not exist");
      throw std::runtime_error(
          "A driver different than avatar driver has claimed the interface\n");
      break;

    case LIBUSB_ERROR_BUSY:
      ALERT("Device",
            "A driver different than avatar driver has claimed the interface");
      throw std::runtime_error(
          "A driver different than avatar driver has claimed the interface\n");
      break;

    case LIBUSB_ERROR_NO_DEVICE:
      ALERT("Device",
            "Avatar driver detected that the device has been disconnected");
      throw std::runtime_error(
          "Avatar driver detected that the device has been disconnected\n");
      break;

    default:
      ALERT("Device", "Avatar driver was not able to claimed interface...");
      throw std::runtime_error(
          "Avatar driver was not able to claimed interface...\n");
      break;
    }
    USBDevice::handle = NULL;
    return;
  }

  // libusb_clear_halt(this->handle, 0x81);
  // libusb_clear_halt(this->handle, 0x01);

  return;
}

uint32_t USBDevice::io(uint8_t endpoint, uint8_t *buffer, uint32_t size) {

  int32_t retval;
  int32_t transferred;
  int32_t attempt = 0;

  do {
    if ((retval = libusb_bulk_transfer(USBDevice::handle, endpoint, buffer, size,
                                       &transferred, timeout)) != 0) {
      ALERT("Device", "%s", libusb_error_name(retval));

      switch (retval) {
      case LIBUSB_ERROR_TIMEOUT:
        ALERT("Device", "USB driver failed : timeout error");
        break;
      case LIBUSB_ERROR_PIPE:
        ALERT("Device", "USB driver failed : pipe error");
        break;
      case LIBUSB_ERROR_OVERFLOW:
        ALERT("Device", "USB driver failed : overflow");
        break;
      case LIBUSB_ERROR_NO_DEVICE:
        ALERT("Device", "USB driver failed : no device");
        break;
      default:
        ALERT("Device", "USB driver failed : error code %d ", retval);
        break;
      }
      sleep(2);
      attempt++;
    } else if (size == 0)
      attempt++;
    else
      break;
  } while (attempt < 6);

  if (attempt >= 5) {
    ALERT("Device",
          "Avatar driver failed to communicate with device ... endpoint : %02x",
          endpoint);
    throw std::runtime_error(
        "Avatar driver failed to communicate with device ... endpoint\n");
  }

  return transferred;
}

void USBDevice::download(uint8_t *data, uint32_t *size) {

  std::stringstream info;

  #ifdef DEBUG
  info << "0x" << std::hex << std::setfill('0');
  for (unsigned int i = 0; i < *size; i++) {
    info << std::setw(2) << static_cast<unsigned>(data[i]);
    info << " ";
  }

  VVERBOSE("JTAG", ">%s", info.str().c_str());
  #endif

  *size = io(entrypoint_download, data, *size);
}

void USBDevice::upload(uint8_t *data, uint32_t *size) {

  std::stringstream info;

  *size = io(entrypoint_upload, data, *size);

  #ifdef DEBUG
  info << "0x" << std::hex << std::setfill('0');
  for (unsigned int i = 0; i < *size; i++) {
    info << std::setw(2) << static_cast<unsigned>(data[i]);
    info << " ";
  }
  VVERBOSE("JTAG", "<%s", info.str().c_str());
  #endif

}
}
