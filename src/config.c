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

void init_config(struct app_config *config) {
  config->baudrate =
      get_env_or_default_long("TERMUX_FTDI_BAUDRATE", BAUDRATE);

  const char *bits_str = getenv("TERMUX_FTDI_BITS");
  config->bits = bits_str ? map_bits_to_enum(bits_str) : BITS;

  const char *stop_bit_str = getenv("TERMUX_FTDI_STOP_BIT");
  config->stop_bit = stop_bit_str ? map_stop_bit_to_enum(stop_bit_str) : STOP_BIT;

  const char *parity_str = getenv("TERMUX_FTDI_PARITY");
  config->parity = parity_str ? map_parity_to_enum(parity_str) : PARITY;

  const char *flow_ctrl_str = getenv("TERMUX_FTDI_FLOW_CTRL");
  config->flow_ctrl =
      flow_ctrl_str ? map_flow_ctrl_to_enum(flow_ctrl_str) : FLOW_CTRL;

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
