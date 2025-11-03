#ifndef CONFIG_H
#define CONFIG_H

#include <ftdi.h>

struct app_config {
  long baudrate;
  int bits;
  int stop_bit;
  int parity;
  int flow_ctrl;
  int latency_timer;
  int buffer_size;
  long select_timeout_us;
  const char *shell_path;
  const char *shell_args;
};

void init_config(struct app_config *config);

#endif // CONFIG_H