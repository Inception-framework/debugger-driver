#include "Device.h"

namespace Device {

USBDevice::USBDevice(uint16_t vid, uint16_t pid, uint32_t interface) {

  this->vid = vid;

  this->pid = pid;

  this->interface = interface;

  this->buffer_limit = 1024;

  this->size = 0;
}

USBDevice::~USBDevice() {
  // TODO Auto-generated destructor stub
}

void USBDevice::change_gpif(void) {}

void USBDevice::quit(void) { this->jtag_libusb_close(); }

bool USBDevice::string_descriptor_equal(libusb_device_handle *device,
                                        uint8_t str_index, const char *string) {
  int retval;
  bool matched;
  char desc_string[256 + 1]; /* Max size of string descriptor */

  if (str_index == 0)
    return false;

  retval = libusb_get_string_descriptor_ascii(
      device, str_index, (unsigned char *)desc_string, sizeof(desc_string) - 1);
  if (retval < 0) {
    ALERT("Device", "libusb_get_string_descriptor_ascii() failed with %d",
          retval);
    return false;
  }

  /* Null terminate descriptor string in case it needs to be logged. */
  desc_string[sizeof(desc_string) - 1] = '\0';

  matched = strncmp(string, desc_string, sizeof(desc_string)) == 0;

  return matched;
}

void USBDevice::jtag_libusb_open(const uint16_t vid, const uint16_t pid,
                                 const char *serial,
                                 struct libusb_device_handle **out) {
  int cnt, idx, errCode;
  struct libusb_device_handle *libusb_handle = NULL;

  if (libusb_init(&this->context) < 0)
    return;

  cnt = libusb_get_device_list(this->context, &devs);

  for (idx = 0; idx < cnt; idx++) {
    struct libusb_device_descriptor dev_desc;

    if (libusb_get_device_descriptor(devs[idx], &dev_desc) != 0)
      continue;

    if (dev_desc.idVendor != vid && dev_desc.idProduct != pid) {
      continue;
    }

    errCode = libusb_open(devs[idx], &libusb_handle);

    if (errCode) {
      ALERT("Device", "libusb_open() failed with %s",
            libusb_error_name(errCode));
      continue;
    }

    /* Device must be open to use libusb_get_string_descriptor_ascii. */
    if (serial != NULL &&
        !string_descriptor_equal(libusb_handle, dev_desc.iSerialNumber,
                                 serial)) {
      libusb_close(libusb_handle);
      continue;
    }

    /* Success. */
    *out = libusb_handle;
    break;
  }
  if (cnt >= 0)
    libusb_free_device_list(devs, 1);
}

void USBDevice::jtag_libusb_close() {
  /* Close device */
  libusb_close(this->handle);

  libusb_exit(this->context);
}

void USBDevice::init(void) {

  int32_t retval;

  this->jtag_libusb_open(this->vid, this->pid, NULL, &this->handle);
  if (!this->handle) {
    ALERT("Device", "Avatar driver doesn't find device\n");
    throw std::runtime_error("Avatar driver doesn't find device\n");
    return;
  }

  this->dev = libusb_get_device(this->handle);

  struct libusb_device_descriptor desciptor;
  libusb_get_device_descriptor(this->dev, &desciptor);

  this->vid = desciptor.idVendor;
  this->pid = desciptor.idProduct;
  this->is_open = 1;
  this->busnum = libusb_get_bus_number(this->dev);
  this->devaddr = libusb_get_device_address(this->dev);

  retval = libusb_claim_interface(this->handle, this->interface);
  if (retval == 0) {
    INFO("Device", "Avatar USB3 driver successfully claimed interface");
  } else {
    switch (retval) {
    case LIBUSB_ERROR_NOT_FOUND:
      ALERT("Device",
            "The interface requested by avatar driver does not exist");
      break;

    case LIBUSB_ERROR_BUSY:
      ALERT("Device",
            "A driver different than avatar driver has claimed the interface");
      break;

    case LIBUSB_ERROR_NO_DEVICE:
      ALERT("Device",
            "Avatar driver detected that the device has been disconnected");
      break;

    default:
      ALERT("Device", "Avatar driver was not able to claimed interface...");
      break;
    }
    this->handle = NULL;
    return;
  }

  return;
}

void USBDevice::io(uint8_t endpoint) {

  int32_t retval;
  int32_t transferred;
  int32_t attempt = 0;

  do {
    if ((retval = libusb_bulk_transfer(this->handle, endpoint, this->buffer,
                                       this->size, &transferred, 1)) != 0) {
      ALERT("Device", libusb_error_name(retval));

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
  } while (attempt < 6);

  if (attempt >= 6) {
    ALERT("Device",
          "Avatar driver failed to communicate with device ... 6 attempts");
    // exit(-1);
  }

  return;
}

void USBDevice::download(uint8_t *data, uint32_t size) {

  this->buffer = data;

  this->size = size;

  this->io(0x1);
}

void USBDevice::upload(uint8_t *data, uint32_t size) {

  this->buffer = data;

  this->size = size;

  this->io(0x81);
}
}
