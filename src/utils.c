#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

const char *get_env_or_default(const char *name, const char *default_value) {
  const char *value = getenv(name);
  return value ? value : default_value;
}

long get_env_or_default_long(const char *name, long default_value) {
  const char *value_str = getenv(name);
  if (value_str) {
    char *endptr;
    long value = strtol(value_str, &endptr, 10);
    if (*endptr == '\0') {
      return value;
    } else {
      fprintf(stderr, "ERROR: Invalid value for %s: '%s'. Not a valid integer.\n", name, value_str);
      exit(EXIT_FAILURE);
    }
  }
  return default_value;
}

const char *get_env_or_default_string(const char *name, const char *default_value) {
  const char *value = getenv(name);
  return value ? value : default_value;
}