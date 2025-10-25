bin_PROGRAMS = termux-ftdi-shell

termux_ftdi_shell_SOURCES = main.c ftdi_utils.c pty_utils.c queue.c

termux_ftdi_shell_CFLAGS = -I$(top_srcdir) $(LIBFTDI1_CFLAGS) $(LIBUSB_CFLAGS)

termux_ftdi_shell_LDADD = $(LIBFTDI1_LIBS) $(LIBUSB_LIBS)
