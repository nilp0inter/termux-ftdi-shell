AC_INIT([termux-ftdi-shell], [1.0], [your-email@example.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

AC_CHECK_LIB([ftdi1], [ftdi_new])
AC_CHECK_LIB([usb-1.0], [libusb_init])

AC_ARG_WITH([baudrate],
  [AS_HELP_STRING([--with-baudrate], [Set the baudrate])],
  [BAUDRATE="$withval"],
  [BAUDRATE="115200"])
AC_DEFINE_UNQUOTED(BAUDRATE, $BAUDRATE, [Baudrate])

AC_ARG_WITH([shell-path],
  [AS_HELP_STRING([--with-shell-path], [Set the shell path])],
  [SHELL_PATH="$withval"],
  [SHELL_PATH="/bin/bash"])
AC_DEFINE_UNQUOTED(SHELL_PATH, "$SHELL_PATH", [Shell path])

AC_ARG_WITH([shell-args],
  [AS_HELP_STRING([--with-shell-args], [Set the shell arguments])],
  [SHELL_ARGS="$withval"],
  [SHELL_ARGS="-i"])
AC_DEFINE_UNQUOTED(SHELL_ARGS, "$SHELL_ARGS", [Shell arguments])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
