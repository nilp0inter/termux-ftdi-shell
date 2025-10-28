bin_PROGRAMS = termux-ftdi-shell

termux_ftdi_shell_SOURCES = main.c ftdi_utils.c pty_utils.c queue.c

termux_ftdi_shell_CFLAGS = -I$(top_srcdir)

termux_ftdi_shell_LDADD = -Wl,-Bstatic -lftdi1 -Wl,-Bdynamic -lusb-1.0
