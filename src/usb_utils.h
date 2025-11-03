#ifndef USB_UTILS_H
#define USB_UTILS_H

#include <stddef.h>

int find_usb_device(int vendor_id, int product_id, char *device_path,
                    size_t device_path_len);

#endif // USB_UTILS_H
