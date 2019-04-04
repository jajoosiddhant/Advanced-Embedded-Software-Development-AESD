#ifndef _LOGGER_H
#define _LOGGER_H

#include "main.h"
#include "queue.h"


//Function Declarations
void log_error(char *str, int errno);
void log_data(sensor_struct data_rcv);


#endif
