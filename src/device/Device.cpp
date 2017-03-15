/**
* @Author: Nassim
* @Date:   2017-03-15T09:52:22+01:00
* @Email:  nassim.corteggiani@maximintegrated.com
* @Project: Inception-commander
* @Last modified by:   Nassim
* @Last modified time: 2017-03-15T15:21:37+01:00
*/



#include "Device.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace Device {

USBDevice::USBDevice(uint16_t p_vid, uint16_t p_pid, uint32_t p_interface) {

  vid = p_vid;

  pid = p_pid;

  interface = p_interface;

  this->buffer_limit = 1024;

  this->handle = NULL;
}

USBDevice::~USBDevice() {
  // TODO Auto-generated destructor stub
}

void USBDevice::close(void) { this->device_close(); }

void USBDevice::device_open() {

  int cnt, idx, errCode;
  libusb_device **devs;

  if (libusb_init(&this->context) < 0)
    return;

  cnt = libusb_get_device_list(this->context, &devs);

  for (idx = 0; idx < cnt; idx++) {

    VVERBOSE("Device","Detecting device idVendor = %02x (Expected idProduct = %02x)",descriptor.idVendor, descriptor.idProduct);

    if (libusb_get_device_descriptor(devs[idx], &descriptor) != 0)
      continue;

    if (descriptor.idVendor != vid || descriptor.idProduct != pid) {
      continue;
    }

    dev = devs[idx];

    errCode = libusb_open(devs[idx], &this->handle);

    if (errCode) {
      this->handle = NULL;
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
  libusb_close(this->handle);

  libusb_exit(this->context);
}

void USBDevice::init(void) {

  int32_t retval;

  device_open();

  if (!this->handle) {
    ALERT("Device", "Avatar driver doesn't find device %04x:%04x \n", vid, pid);
    throw std::runtime_error("Avatar driver doesn't find device\n");
    return;
  }

  dev = libusb_get_device(handle);

  if(vid != descriptor.idVendor) {

    ALERT("DEVICE", " ID Vendor %04x:%04x", vid, descriptor.idVendor);
    exit(0);
  }

  if(pid != descriptor.idProduct) {

    ALERT("DEVICE", " ID Product %04x:%04x", pid, descriptor.idProduct);
    exit(0);
  }

  is_open = 1;

  busnum = libusb_get_bus_number(dev);
  devaddr = libusb_get_device_address(dev);

  retval = libusb_claim_interface(handle, interface);
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
    this->handle = NULL;
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
    if ((retval = libusb_bulk_transfer(this->handle, endpoint, buffer, size,
                                       &transferred, 0)) != 0) {
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
    } else
      break;
  } while (attempt < 2);

  if (attempt >= 2) {
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

  info << "0x" << std::hex << std::setfill('0');
  for (unsigned int i = 0; i < *size; i++) {
    info << std::setw(2) << static_cast<unsigned>(data[i]);
    //VVERBOSE("JTAG", "%02x", data[i]);
  }

  VVERBOSE("JTAG", "%s", info.str().c_str());

  *size = this->io(0x01, data, *size);
}

void USBDevice::upload(uint8_t *data, uint32_t *size) {

  *size = this->io(0x81, data, *size);
}
}
