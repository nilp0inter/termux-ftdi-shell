#include "usb_utils.h"
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <string.h>

int find_usb_device(int vendor_id, int product_id, char *device_path,
                    size_t device_path_len) {
  libusb_device **devs;
  libusb_context *ctx = NULL;
  int r;
  ssize_t cnt;
  int result = -1;

  r = libusb_init(&ctx);
  if (r < 0) {
    fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(r));
    return -1;
  }

  cnt = libusb_get_device_list(ctx, &devs);
  if (cnt < 0) {
    fprintf(stderr, "libusb_get_device_list failed: %s\n",
            libusb_error_name(cnt));
    libusb_exit(ctx);
    return -1;
  }

  for (ssize_t i = 0; i < cnt; i++) {
    struct libusb_device_descriptor desc;
    r = libusb_get_device_descriptor(devs[i], &desc);
    if (r < 0) {
      fprintf(stderr, "failed to get device descriptor");
      continue;
    }

    if (desc.idVendor == vendor_id && desc.idProduct == product_id) {
      uint8_t bus = libusb_get_bus_number(devs[i]);
      uint8_t device = libusb_get_device_address(devs[i]);
      snprintf(device_path, device_path_len, "/dev/bus/usb/%03d/%03d", bus,
               device);
      result = 0;
      break;
    }
  }

  libusb_free_device_list(devs, 1);
  libusb_exit(ctx);
  return result;
}
