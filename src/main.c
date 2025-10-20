#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libusb-1.0/libusb.h>
#include <ftdi.h>
#include <pty.h>
#include <unistd.h>
#include <sys/select.h>

#define BAUDRATE 115200

int main(int argc, char **argv) {
    libusb_context *usb_context;
    libusb_device_handle *usb_handle;
    libusb_device *usb_dev;
    struct libusb_device_descriptor desc;
    unsigned char buffer[256];
    int fd;

    struct ftdi_context *ftdi;
    struct ftdi_version_info version;
    int pty_master;
    pid_t pid;

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

    // Configure FTDI device for serial communication
    if (ftdi_set_baudrate(ftdi, BAUDRATE) < 0) {
        fprintf(stderr, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(ftdi));
        ftdi->usb_dev = NULL;
        ftdi_free(ftdi);
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }
    if (ftdi_set_line_property(ftdi, BITS_8, STOP_BIT_1, NONE) < 0) {
        fprintf(stderr, "ftdi_set_line_property failed: %s\n", ftdi_get_error_string(ftdi));
        ftdi->usb_dev = NULL;
        ftdi_free(ftdi);
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }
    if (ftdi_setflowctrl(ftdi, SIO_DISABLE_FLOW_CTRL) < 0) {
        fprintf(stderr, "ftdi_setflowctrl failed: %s\n", ftdi_get_error_string(ftdi));
        ftdi->usb_dev = NULL;
        ftdi_free(ftdi);
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }

    // Create a pseudo-terminal and fork
    pid = forkpty(&pty_master, NULL, NULL, NULL);
    if (pid < 0) {
        perror("forkpty");
        ftdi->usb_dev = NULL;
        ftdi_free(ftdi);
        libusb_close(usb_handle);
        libusb_exit(usb_context);
        return EXIT_FAILURE;
    }

    // Child process: execute a shell
    if (pid == 0) {
        char *shell = "/bin/sh";
        char *args[] = {shell, NULL};
        execv(shell, args);
        perror("execv"); // execv only returns on error
        exit(1);
    }

    // Parent process: communication loop
    fprintf(stderr, "Starting shell...\n");
    while (1) {
        fd_set read_fds;
        int max_fd = 0;
        int activity;

        FD_ZERO(&read_fds);
        FD_SET(pty_master, &read_fds);
        max_fd = pty_master;

        // Add FTDI device to the set
        // Note: libftdi uses libusb's event-driven API, but for simplicity, we'll poll
        // This is not the most efficient way, but it's easier to implement
        unsigned char ftdi_buf[1024];
        int ftdi_ret = ftdi_read_data(ftdi, ftdi_buf, sizeof(ftdi_buf));
        if (ftdi_ret > 0) {
            fprintf(stderr, "Read %d bytes from FTDI\n", ftdi_ret);
            // write(pty_master, ftdi_buf, ftdi_ret);
        }

        FD_SET(pty_master, &read_fds);

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("select");
            break;
        }

        if (FD_ISSET(pty_master, &read_fds)) {
            char pty_buf[1024];
            int pty_ret = read(pty_master, pty_buf, sizeof(pty_buf));
            if (pty_ret > 0) {
                fprintf(stderr, "Read %d bytes from PTY\n", pty_ret);
                // ftdi_write_data(ftdi, (const unsigned char *)pty_buf, pty_ret);
            } else {
                break; // Shell has exited
            }
        }
    }

    fprintf(stderr, "Shell exited.\n");

    // Cleanup
    ftdi->usb_dev = NULL;
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);

    return EXIT_SUCCESS;
}
