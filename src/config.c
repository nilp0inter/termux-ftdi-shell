#include "config.h"
#include "../config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static int map_bits_to_enum(const char *value) {
  if (strcmp(value, "BITS_7") == 0) {
    return BITS_7;
  } else if (strcmp(value, "BITS_8") == 0) {
    return BITS_8;
  } else {
    fprintf(stderr, "ERROR: Invalid value for TERMUX_FTDI_BITS: '%s'. Valid values are 'BITS_7' and 'BITS_8'.\n", value);
    exit(EXIT_FAILURE);
  }
}

static int map_stop_bit_to_enum(const char *value) {
  if (strcmp(value, "STOP_BIT_1") == 0) {
    return STOP_BIT_1;
  } else if (strcmp(value, "STOP_BIT_2") == 0) {
    return STOP_BIT_2;
  } else {
    fprintf(stderr, "ERROR: Invalid value for TERMUX_FTDI_STOP_BIT: '%s'. Valid values are 'STOP_BIT_1' and 'STOP_BIT_2'.\n", value);
    exit(EXIT_FAILURE);
  }
}

static int map_parity_to_enum(const char *value) {
  if (strcmp(value, "ODD") == 0) {
    return ODD;
  } else if (strcmp(value, "EVEN") == 0) {
    return EVEN;
  } else if (strcmp(value, "MARK") == 0) {
    return MARK;
  } else if (strcmp(value, "SPACE") == 0) {
    return SPACE;
  } else if (strcmp(value, "NONE") == 0) {
    return NONE;
  } else {
    fprintf(stderr, "ERROR: Invalid value for TERMUX_FTDI_PARITY: '%s'. Valid values are 'NONE', 'ODD', 'EVEN', 'MARK', and 'SPACE'.\n", value);
    exit(EXIT_FAILURE);
  }
}

static int map_flow_ctrl_to_enum(const char *value) {
  if (strcmp(value, "SIO_RTS_CTS_HS") == 0) {
    return SIO_RTS_CTS_HS;
  } else if (strcmp(value, "SIO_DTR_DSR_HS") == 0) {
    return SIO_DTR_DSR_HS;
  } else if (strcmp(value, "SIO_XON_XOFF_HS") == 0) {
    return SIO_XON_XOFF_HS;
  } else if (strcmp(value, "SIO_DISABLE_FLOW_CTRL") == 0) {
    return SIO_DISABLE_FLOW_CTRL;
  } else {
    fprintf(stderr, "ERROR: Invalid value for TERMUX_FTDI_FLOW_CTRL: '%s'. Valid values are 'SIO_DISABLE_FLOW_CTRL', 'SIO_RTS_CTS_HS', 'SIO_DTR_DSR_HS', and 'SIO_XON_XOFF_HS'.\n", value);
    exit(EXIT_FAILURE);
  }
}

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

void init_config(struct app_config *config) {
  config->baudrate =
      get_env_or_default_long("TERMUX_FTDI_BAUDRATE", BAUDRATE);
  config->bits =
      map_bits_to_enum(get_env_or_default_string("TERMUX_FTDI_BITS", TO_STRING(BITS)));
  config->stop_bit = map_stop_bit_to_enum(
      get_env_or_default_string("TERMUX_FTDI_STOP_BIT", TO_STRING(STOP_BIT)));
  config->parity = map_parity_to_enum(
      get_env_or_default_string("TERMUX_FTDI_PARITY", TO_STRING(PARITY)));
  config->flow_ctrl = map_flow_ctrl_to_enum(
      get_env_or_default_string("TERMUX_FTDI_FLOW_CTRL", TO_STRING(FLOW_CTRL)));
  config->latency_timer =
      get_env_or_default_long("TERMUX_FTDI_LATENCY_TIMER", LATENCY_TIMER);
  config->buffer_size =
      get_env_or_default_long("TERMUX_FTDI_BUFFER_SIZE", BUFFER_SIZE);
  config->select_timeout_us =
      get_env_or_default_long("TERMUX_FTDI_SELECT_TIMEOUT_US", SELECT_TIMEOUT_US);
  config->shell_path =
      get_env_or_default_string("TERMUX_FTDI_SHELL_PATH", SHELL_PATH);
  config->shell_args = get_env_or_default_string("TERMUX_FTDI_SHELL_ARGS", SHELL_ARGS);
}
