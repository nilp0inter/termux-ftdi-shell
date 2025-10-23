/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Baudrate */
#define BAUDRATE 115200

/* Bits */
#define BITS BITS_8

/* Buffer size */
#define BUFFER_SIZE 1024

/* Flow control */
#define FLOW_CTRL SIO_DISABLE_FLOW_CTRL

/* Define to 1 if you have the `ftdi1' library (-lftdi1). */
/* #undef HAVE_LIBFTDI1 */

/* Define to 1 if you have the `usb-1.0' library (-lusb-1.0). */
/* #undef HAVE_LIBUSB_1_0 */

/* Latency timer */
#define LATENCY_TIMER 1

/* Name of package */
#define PACKAGE "termux-ftdi-shell"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "your-email@example.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "termux-ftdi-shell"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "termux-ftdi-shell 1.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "termux-ftdi-shell"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0"

/* Parity */
#define PARITY NONE

/* Select timeout in microseconds */
#define SELECT_TIMEOUT_US 10000

/* Shell arguments */
#define SHELL_ARGS "-i"

/* Shell path */
#define SHELL_PATH "/bin/bash"

/* Stop bit */
#define STOP_BIT STOP_BIT_1

/* Version number of package */
#define VERSION "1.0"
