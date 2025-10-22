bin_PROGRAMS = termux-ftdi-shell

termux_ftdi_shell_SOURCES = main.c ftdi_utils.c pty_utils.c queue.c

termux_ftdi_shell_CFLAGS = -I$(top_srcdir) -I/data/data/com.termux/files/usr/include/libftdi1

termux_ftdi_shell_LDFLAGS = -lftdi1 -lusb-1.0
