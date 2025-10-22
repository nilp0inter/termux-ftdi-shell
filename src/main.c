#include "ftdi_utils.h"
#include "pty_utils.h"
#include "queue.h"
#include <assert.h>
#include <errno.h>
#include <ftdi.h>
#include <libusb-1.0/libusb.h>
#include <poll.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

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

  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  if (libusb_init(&usb_context) != 0) {
    fprintf(stderr, "libusb_init failed\n");
    return EXIT_FAILURE;
  }

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

  if (libusb_get_string_descriptor_ascii(usb_handle, desc.iManufacturer, buffer,
                                         256) >= 0) {
    fprintf(stderr, "Manufacturer: %s\n", buffer);
  }
  if (libusb_get_string_descriptor_ascii(usb_handle, desc.iProduct, buffer,
                                         256) >= 0) {
    fprintf(stderr, "Product: %s\n", buffer);
  }
  if (libusb_get_string_descriptor_ascii(usb_handle, desc.iSerialNumber, buffer,
                                         256) >= 0) {
    fprintf(stderr, "Serial No: %s\n", buffer);
  }

  if ((ftdi = ftdi_new()) == 0) {
    fprintf(stderr, "ftdi_new failed\n");
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }

  version = ftdi_get_library_version();
  fprintf(stderr,
          "Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot "
          "ver: %s)\n",
          version.version_str, version.major, version.minor, version.micro,
          version.snapshot_str);

  if (libusb_kernel_driver_active(usb_handle, 0)) {
    libusb_detach_kernel_driver(usb_handle, 0);
    fprintf(stderr, "Detached kernel driver\n");
  }

  if (ftdi_usb_open_from_wrapped_device(ftdi, usb_context, usb_handle, &desc) <
      0) {
    fprintf(stderr, "ftdi_usb_open_from_wrapped_device failed\n");
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }

  if (ftdi_set_baudrate(ftdi, BAUDRATE) < 0) {
    fprintf(stderr, "ftdi_set_baudrate failed: %s\n",
            ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }
  if (ftdi_set_line_property(ftdi, BITS_8, STOP_BIT_1, NONE) < 0) {
    fprintf(stderr, "ftdi_set_line_property failed: %s\n",
            ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }
  if (ftdi_setflowctrl(ftdi, SIO_DISABLE_FLOW_CTRL) < 0) {
    fprintf(stderr, "ftdi_setflowctrl failed: %s\n",
            ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }

  if (ftdi_set_latency_timer(ftdi, 1) < 0) {
    fprintf(stderr, "ftdi_set_latency_timer failed: %s\n",
            ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    libusb_close(usb_handle); // This line seems to have a typo, assuming it
                              // should be usb_handle
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }

  pid = setup_pty(&pty_master);
  if (pid < 0) {
    ftdi->usb_dev = NULL;
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);
    return EXIT_FAILURE;
  }

  fprintf(stderr, "Starting shell...\n");

  struct ftdi_transfer_control *write_tc = NULL;
  unsigned char read_buf[1024];
  Queue *write_queue = queue_create();

  while (1) {
    fd_set read_fds, write_fds, except_fds;
    int max_fd = 0;
    struct timeval tv;

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);

    FD_SET(pty_master, &read_fds);
    max_fd = pty_master;

    const struct libusb_pollfd **pollfds = libusb_get_pollfds(usb_context);
    if (pollfds) {
      for (int i = 0; pollfds[i] != NULL; i++) {
        if (pollfds[i]->events & POLLIN)
          FD_SET(pollfds[i]->fd, &read_fds);
        if (pollfds[i]->events & POLLOUT)
          FD_SET(pollfds[i]->fd, &write_fds);
        if (pollfds[i]->fd > max_fd)
          max_fd = pollfds[i]->fd;
      }
    }

    tv.tv_sec = 0;
    tv.tv_usec = 10000; // 10ms timeout

    int activity = select(max_fd + 1, &read_fds, &write_fds, &except_fds, &tv);

    if (activity < 0) {
      if (errno == EINTR)
        continue;
      perror("select");
      break;
    }

    struct timeval zero_tv = {0, 0};
    if (libusb_handle_events_timeout(usb_context, &zero_tv) < 0) {
      fprintf(stderr, "libusb_handle_events_timeout failed\n");
      break;
    }

    if (FD_ISSET(pty_master, &read_fds)) {
      char pty_buf[1024];
      int pty_ret = read(pty_master, pty_buf, sizeof(pty_buf));
      if (pty_ret > 0) {
        if (queue_enqueue(write_queue, (unsigned char *)pty_buf, pty_ret) !=
            0) {
          fprintf(stderr, "Failed to enqueue PTY data\n");
        }
      } else {
        break;
      }
    }

    if (!write_tc && write_queue->head) {
      QueueNode *node = queue_dequeue(write_queue);
      if (node) {
        write_tc = ftdi_write_data_submit(ftdi, node->data, node->len);
        if (!write_tc) {
          fprintf(stderr, "ftdi_write_data_submit failed\n");
        }
        // The FTDI library takes ownership of the buffer and will free it,
        // so we only need to free the queue node itself.
        free(node);
      }
    }

    int ftdi_ret = ftdi_read_data(ftdi, read_buf, sizeof(read_buf));
    if (ftdi_ret > 0) {
      write(pty_master, read_buf, ftdi_ret);
    }

    if (write_tc) {
      int bytes_done = ftdi_transfer_data_done(write_tc);
      if (bytes_done >= 0) {
        free(write_tc->buf);
        write_tc = NULL;
      } else if (bytes_done < 0) {
        fprintf(stderr, "Write transfer failed: %d\n", bytes_done);
        free(write_tc->buf);
        write_tc = NULL;
      }
    }

    if (pollfds) {
      libusb_free_pollfds(pollfds);
    }
  }

    fprintf(stderr, "Shell exited.\n");

    queue_destroy(write_queue);
    ftdi->usb_dev = NULL;
    ftdi_free(ftdi);
    libusb_close(usb_handle);
    libusb_exit(usb_context);

    return EXIT_SUCCESS;
}
