#ifndef CONFIG_H
#define CONFIG_H

// FTDI settings
#define BAUDRATE 115200
#define BITS BITS_8
#define STOP_BIT STOP_BIT_1
#define PARITY NONE
#define FLOW_CTRL SIO_DISABLE_FLOW_CTRL
#define LATENCY_TIMER 1
#define BUFFER_SIZE 1024
#define SELECT_TIMEOUT_US 10000

// PTY settings
#define SHELL_PATH "/bin/bash"
#define SHELL_ARGS "-i"

#endif // CONFIG_H
