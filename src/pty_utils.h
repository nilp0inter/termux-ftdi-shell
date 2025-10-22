#ifndef PTY_UTILS_H
#define PTY_UTILS_H

#include <unistd.h>

pid_t setup_pty(int *pty_master);

#endif // PTY_UTILS_H
