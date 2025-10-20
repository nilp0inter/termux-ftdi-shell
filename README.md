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
make
```

## Usage

```bash
./termux-ftdi-shell
```
