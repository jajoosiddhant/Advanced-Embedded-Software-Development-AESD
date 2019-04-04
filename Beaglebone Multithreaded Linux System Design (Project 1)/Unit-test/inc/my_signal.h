#ifndef _MY_SIGNAL_H
#define _MY_SIGNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "queue.h"
#include "main.h"

err_t sig_init(void);
void signal_handler(int signo, siginfo_t *info, void *extra);

#endif
