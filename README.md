# termux-ftdi-shell

Expose a command (usually a shell) through a FTDI usb to serial adapter in any non-rooted android device.

This project uses `termux-usb`, `libusb` and `libftdi` to create a simple tool that can be used to access a shell on an Android device from a computer using a USB to serial adapter.

## Installation

1.  Add the repository to Termux:
    ```bash
    mkdir -p $PREFIX/etc/apt/sources.list.d
    echo "deb [trusted=yes] https://nilp0inter.github.io/termux-ftdi-shell/deb stable main" > $PREFIX/etc/apt/sources.list.d/termux-ftdi-shell.list
    ```

2.  Update package list:
    ```bash
    pkg update
    ```

3.  Install the package:
    ```bash
    pkg install termux-ftdi-shell
    ```

## Dependencies

This package automatically installs `libusb` from Termux repositories. `libftdi` is statically linked and included in the binary.

## Usage

To run the application, you can use the provided example script. This script will find the correct USB device and launch the shell.

```bash
./run-termux-ftdi-shell
```

### Environment Variables

You can set the following environment variables to configure the application at runtime:

| Variable | Description | Default |
| --- | --- | --- |
| `TERMUX_FTDI_BAUDRATE` | Set the baudrate | `115200` |
| `TERMUX_FTDI_BITS` | Set the bits | `8` |
| `TERMUX_FTDI_STOP_BIT` | Set the stop bit | `1` |
| `TERMUX_FTDI_PARITY` | Set the parity | `NONE` |
| `TERMUX_FTDI_FLOW_CTRL` | Set the flow control | `SIO_DISABLE_FLOW_CTRL` |
| `TERMUX_FTDI_LATENCY_TIMER` | Set the latency timer | `1` |
| `TERMUX_FTDI_BUFFER_SIZE` | Set the buffer size | `1024` |
| `TERMUX_FTDI_SELECT_TIMEOUT_US` | Set the select timeout in microseconds | `10000` |
| `TERMUX_FTDI_SHELL_PATH` | Set the shell path | `/bin/bash` |
| `TERMUX_FTDI_SHELL_ARGS` | Set the shell arguments | `-i` |

## Building from Source

### Setup

To install the required dependencies, run the following command in your Termux environment:

```bash
./setup_termux
```

This will install the necessary packages and build `libftdi` from source.

### Building

```bash
autoreconf -i
./configure
make
```

### Configure Options

You can set the following options when running `./configure`:

| Option                  | Description                              | Default                   |
| ----------------------- | ---------------------------------------- | ------------------------- |
| `--with-baudrate`       | Set the baudrate                         | `115200`                  |
| `--with-bits`           | Set the bits                             | `BITS_8`                  |
| `--with-stop-bit`       | Set the stop bit                         | `STOP_BIT_1`              |
| `--with-parity`         | Set the parity                           | `NONE`                    |
| `--with-flow-ctrl`      | Set the flow control                     | `SIO_DISABLE_FLOW_CTRL`   |
| `--with-latency-timer`  | Set the latency timer                    | `1`                       |
| `--with-buffer-size`    | Set the buffer size                      | `1024`                    |
| `--with-select-timeout-us` | Set the select timeout in microseconds   | `10000`                   |
| `--with-shell-path`     | Set the shell path                       | `/bin/bash`               |
| `--with-shell-args`     | Set the shell arguments                  | `-i`                      |
