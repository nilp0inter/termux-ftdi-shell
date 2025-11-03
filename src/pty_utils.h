#ifndef PTY_UTILS_H
#define PTY_UTILS_H

#include <unistd.h>

pid_t setup_pty(int *pty_master, const char *shell_path, const char *shell_args);

#endif // PTY_UTILS_H
