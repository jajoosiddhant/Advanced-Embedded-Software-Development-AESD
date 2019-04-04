/**
 * @file logger.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file of logger.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef _LOGGER_H
#define _LOGGER_H

#include "main.h"
#include "queue.h"


#define UNIT ((TEMP_UNIT == 0)? "Celsius": (TEMP_UNIT == 1)? "Kelvin": (TEMP_UNIT == 2)? "Fahrenheit": "")

//Function Declarations
void log_data(sensor_struct data_rcv);


#endif
