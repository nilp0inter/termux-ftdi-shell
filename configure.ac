AC_INIT([termux-ftdi-shell], [1.0], [https://github.com/nilp0inter/termux-ftdi-shell/issues], [termux-ftdi-shell], [https://github.com/nilp0inter/termux-ftdi-shell])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

AC_CHECK_LIB([ftdi1], [ftdi_new])
AC_CHECK_LIB([usb-1.0], [libusb_init])

AC_ARG_WITH([baudrate],
  [AS_HELP_STRING([--with-baudrate], [Set the baudrate])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-baudrate option requires an argument])
    ])
    BAUDRATE="$withval"
  ],
  [BAUDRATE="115200"])
AC_DEFINE_UNQUOTED(BAUDRATE, $BAUDRATE, [Baudrate])

AC_ARG_WITH([bits],
  [AS_HELP_STRING([--with-bits], [Set the bits])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-bits option requires an argument])
    ])
    BITS="$withval"
  ],
  [BITS="BITS_8"])
AC_DEFINE_UNQUOTED(BITS, $BITS, [Bits])

AC_ARG_WITH([stop-bit],
  [AS_HELP_STRING([--with-stop-bit], [Set the stop bit])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-stop-bit option requires an argument])
    ])
    STOP_BIT="$withval"
  ],
  [STOP_BIT="STOP_BIT_1"])
AC_DEFINE_UNQUOTED(STOP_BIT, $STOP_BIT, [Stop bit])

AC_ARG_WITH([parity],
  [AS_HELP_STRING([--with-parity], [Set the parity])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-parity option requires an argument])
    ])
    PARITY="$withval"
  ],
  [PARITY="NONE"])
AC_DEFINE_UNQUOTED(PARITY, $PARITY, [Parity])

AC_ARG_WITH([flow-ctrl],
  [AS_HELP_STRING([--with-flow-ctrl], [Set the flow control])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-flow-ctrl option requires an argument])
    ])
    FLOW_CTRL="$withval"
  ],
  [FLOW_CTRL="SIO_DISABLE_FLOW_CTRL"])
AC_DEFINE_UNQUOTED(FLOW_CTRL, $FLOW_CTRL, [Flow control])

AC_ARG_WITH([latency-timer],
  [AS_HELP_STRING([--with-latency-timer], [Set the latency timer])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-latency-timer option requires an argument])
    ])
    LATENCY_TIMER="$withval"
  ],
  [LATENCY_TIMER="1"])
AC_DEFINE_UNQUOTED(LATENCY_TIMER, $LATENCY_TIMER, [Latency timer])

AC_ARG_WITH([buffer-size],
  [AS_HELP_STRING([--with-buffer-size], [Set the buffer size])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-buffer-size option requires an argument])
    ])
    BUFFER_SIZE="$withval"
  ],
  [BUFFER_SIZE="1024"])
AC_DEFINE_UNQUOTED(BUFFER_SIZE, $BUFFER_SIZE, [Buffer size])

AC_ARG_WITH([select-timeout-us],
  [AS_HELP_STRING([--with-select-timeout-us], [Set the select timeout in microseconds])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-select-timeout-us option requires an argument])
    ])
    SELECT_TIMEOUT_US="$withval"
  ],
  [SELECT_TIMEOUT_US="10000"])
AC_DEFINE_UNQUOTED(SELECT_TIMEOUT_US, $SELECT_TIMEOUT_US, [Select timeout in microseconds])

AC_ARG_WITH([shell-path],
  [AS_HELP_STRING([--with-shell-path], [Set the shell path])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-shell-path option requires an argument])
    ])
    SHELL_PATH="$withval"
  ],
  [SHELL_PATH="/bin/bash"])
AC_DEFINE_UNQUOTED(SHELL_PATH, "$SHELL_PATH", [Shell path])

AC_ARG_WITH([shell-args],
  [AS_HELP_STRING([--with-shell-args], [Set the shell arguments])],
  [
    AS_IF([test "x$withval" = "xyes"], [
      AC_MSG_ERROR([--with-shell-args option requires an argument])
    ])
    SHELL_ARGS="$withval"
  ],
  [SHELL_ARGS="-i"])
AC_DEFINE_UNQUOTED(SHELL_ARGS, "$SHELL_ARGS", [Shell arguments])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
