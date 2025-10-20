#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libusb-1.0/libusb.h>
#include <ftdi.h>

int main(int argc, char **argv) {
    libusb_context *usb_context;
    libusb_device_handle *usb_handle;
    libusb_device *usb_dev;
    struct libusb_device_descriptor desc;
    unsigned char buffer[256];
    int fd;
    int ret;
    struct ftdi_context *ftdi;
    struct ftdi_version_info version;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_descriptor>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (sscanf(argv[1], "%d", &fd) != 1) {
        fprintf(stderr, "Invalid file descriptor: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Initialize libusb
    libusb_set_option(NULL, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
    if (libusb_init(&usb_context) != 0) {
        fprintf(stderr, "libusb_init failed\n");
        return EXIT_FAILURE;
    }

    // Wrap the system device
    if (libusb_wrap_sys_device(usb_context, (intptr_t)fd, &usb_handle) != 0) {
        fprintf(stderr, "libusb_wrap_sys_device failed\n");
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }

    usb_dev = libusb_get_device(usb_handle);
    if (libusb_get_device_descriptor(usb_dev, &desc) != 0) {
        fprintf(stderr, "libusb_get_device_descriptor failed\n");
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Vendor ID: %04x\n", desc.idVendor);
    fprintf(stderr, "Product ID: %04x\n", desc.idProduct);

    if (libusb_get_string_descriptor_ascii(usb_handle, desc.iManufacturer, buffer, 256) >= 0) {
        fprintf(stderr, "Manufacturer: %s\n", buffer);
    }
    if (libusb_get_string_descriptor_ascii(usb_handle, desc.iProduct, buffer, 256) >= 0) {
        fprintf(stderr, "Product: %s\n", buffer);
    }
    if (libusb_get_string_descriptor_ascii(usb_handle, desc.iSerialNumber, buffer, 256) >= 0) {
        fprintf(stderr, "Serial No: %s\n", buffer);
    }

    // Initialize libftdi
    if ((ftdi = ftdi_new()) == 0) {
        fprintf(stderr, "ftdi_new failed\n");
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }

    version = ftdi_get_library_version();
    fprintf(stderr, "Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
           version.version_str, version.major, version.minor, version.micro,
           version.snapshot_str);

    // Assign the device handle to the ftdi context
    ftdi->usb_dev = usb_handle;
    ftdi->type = TYPE_R; // Assuming a modern FTDI chip
    ftdi->interface = INTERFACE_A; // Assuming interface A

    // Read out FTDIChip-ID of R type chips
    if (ftdi->type == TYPE_R) {
        unsigned int chipid;
        if (ftdi_read_chipid(ftdi, &chipid) == 0) {
            fprintf(stderr, "FTDI chipid: %X\n", chipid);
        } else {
            fprintf(stderr, "ftdi_read_chipid failed: %s\n", ftdi_get_error_string(ftdi));
        }
    }

    // ftdi_usb_close is not needed as we are not opening it with ftdi_usb_open
    ftdi->usb_dev = NULL;
    
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);

    return EXIT_SUCCESS;
}
