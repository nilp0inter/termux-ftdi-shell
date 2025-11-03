#include "config.h"
#include "pty_utils.h"
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

pid_t setup_pty(int *pty_master, const char *shell_path,
                  const char *shell_args) {
  pid_t pid = forkpty(pty_master, NULL, NULL, NULL);
  if (pid < 0) {
    perror("forkpty");
    return -1;
  }

  if (pid > 0) {
    struct termios term;
    if (tcgetattr(*pty_master, &term) < 0) {
      perror("tcgetattr");
      return -1;
    }
    cfmakeraw(&term);
    term.c_iflag &= ~ICRNL;
    if (tcsetattr(*pty_master, TCSANOW, &term) < 0) {
      perror("tcsetattr");
      return -1;
    }
  } else if (pid == 0) {
    char *shell = (char *)shell_path;
    char *args[] = {shell, (char *)shell_args, NULL};
    execv(shell, args);
    perror("execv");
    exit(1);
  }

  return pid;
}
