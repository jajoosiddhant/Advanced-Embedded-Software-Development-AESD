/**
 * @file my_signal.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file of my)signal.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef _MY_SIGNAL_H
#define _MY_SIGNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "queue.h"
#include "main.h"
#include "timer.h"

//Function Declarations
err_t sig_init(void);
void signal_handler(int signo, siginfo_t *info, void *extra);

#endif
