/*
 * led.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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


static uint32_t cnt;

//Function Declarations
uint8_t ledtask_init(void);
static void led(void *pvParameters);
void led_config(void);
void led_send(void);

#endif /* INC_LED_H_ */
