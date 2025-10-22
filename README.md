# termux-ftdi-shell

Expose a command (usually a shell) through a FTDI usb to serial adapter in any non-rooted android device.

This project uses `termux-usb`, `libusb` and `libftdi` to create a simple tool that can be used to access a shell on an Android device from a computer using a USB to serial adapter.

## Setup

To install the required dependencies, run the following command in your Termux environment:

```bash
./setup_termux
```

This will install the necessary packages and build `libftdi` from source.

## Building

```bash
autoreconf -i
./configure
make
```

### Configure Options

You can set the following options when running `./configure`:

- `--with-baudrate=RATE`: Set the baudrate (default: 115200)
- `--with-shell-path=PATH`: Set the shell path (default: /bin/bash)
- `--with-shell-args=ARGS`: Set the shell arguments (default: -i)


## Usage

To run the application, you can use the provided example script. This script will find the correct USB device and launch the shell.

```bash
./run-termux-ftdi-shell
```
