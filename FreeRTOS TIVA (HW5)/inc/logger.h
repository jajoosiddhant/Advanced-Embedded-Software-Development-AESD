/*
 * logger.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "sensorlib/i2cm_drv.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"
#include "inc/temp.h"


char buff[30];

//Function Declarations
uint8_t loggertask_init(void);
static void logger(void *pvParameters);
void log(void);


#endif /* INC_LOGGER_H_ */
