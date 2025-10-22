#ifndef FTD_UTILS_H
#define FTD_UTILS_H

#include <ftdi.h>
#include <libusb-1.0/libusb.h>

int ftdi_usb_open_from_wrapped_device(struct ftdi_context *ftdi,
                                      libusb_context *usb_context,
                                      libusb_device_handle *handle,
                                      struct libusb_device_descriptor *desc);

#endif // FTD_UTILS_H
