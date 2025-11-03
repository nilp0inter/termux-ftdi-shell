#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

const char *get_env_or_default(const char *name, const char *default_value);
long get_env_or_default_long(const char *name, long default_value);
const char *get_env_or_default_string(const char *name, const char *default_value);

#endif // UTILS_H