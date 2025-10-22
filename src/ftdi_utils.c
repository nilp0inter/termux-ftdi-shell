#include "ftdi_utils.h"
#include <ftdi.h>
#include <stdio.h>

int ftdi_usb_open_from_wrapped_device(struct ftdi_context *ftdi,
                                      libusb_context *usb_context,
                                      libusb_device_handle *handle,
                                      struct libusb_device_descriptor *desc) {
  if (ftdi == NULL)
    return -8;

  ftdi->usb_dev = handle;
  // Use the same libusb context that has the wrapped device.
  libusb_exit(ftdi->usb_ctx);
  ftdi->usb_ctx = usb_context;

  if (libusb_claim_interface(ftdi->usb_dev, ftdi->interface) < 0) {
    fprintf(stderr, "libusb_claim_interface() failed\n");
    return -5;
  }

  if (ftdi_usb_reset(ftdi) != 0) {
    fprintf(stderr, "ftdi_usb_reset() failed\n");
    return -6;
  }

  if (desc->bcdDevice == 0x400 ||
      (desc->bcdDevice == 0x200 && desc->iSerialNumber == 0))
    ftdi->type = TYPE_BM;
  else if (desc->bcdDevice == 0x200)
    ftdi->type = TYPE_AM;
  else if (desc->bcdDevice == 0x500)
    ftdi->type = TYPE_2232C;
  else if (desc->bcdDevice == 0x600)
    ftdi->type = TYPE_R;
  else if (desc->bcdDevice == 0x700)
    ftdi->type = TYPE_2232H;
  else if (desc->bcdDevice == 0x800)
    ftdi->type = TYPE_4232H;
  else if (desc->bcdDevice == 0x900)
    ftdi->type = TYPE_232H;
  else if (desc->bcdDevice == 0x1000)
    ftdi->type = TYPE_230X;

  unsigned int packet_size;
  if (ftdi->type == TYPE_2232H || ftdi->type == TYPE_4232H ||
      ftdi->type == TYPE_232H)
    packet_size = 512;
  else
    packet_size = 64;

  libusb_device *dev = libusb_get_device(handle);
  if (dev) {
    struct libusb_config_descriptor *config0;
    if (libusb_get_config_descriptor(dev, 0, &config0) == 0) {
      if (desc->bNumConfigurations > 0) {
        if (ftdi->interface < config0->bNumInterfaces) {
          struct libusb_interface interface =
              config0->interface[ftdi->interface];
          if (interface.num_altsetting > 0) {
            struct libusb_interface_descriptor descriptor =
                interface.altsetting[0];
            if (descriptor.bNumEndpoints > 0) {
              packet_size = descriptor.endpoint[0].wMaxPacketSize;
            }
          }
        }
      }
      libusb_free_config_descriptor(config0);
    }
  }
  ftdi->max_packet_size = packet_size;

  if (ftdi_set_baudrate(ftdi, 9600) != 0) {
    fprintf(stderr, "ftdi_set_baudrate() failed\n");
    return -7;
  }

  return 0;
}
