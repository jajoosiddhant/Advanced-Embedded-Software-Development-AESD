/*
 * temp.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#ifndef INC_TEMP_H_
#define INC_TEMP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
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

#define LEDTASKSTACKSIZE        128         // Stack size in words

#define CONFIG_TEMP     (0x00)
#define SLAVE_ADDR      (0x48)          //Slave Address of I2C sensor.
#define THRESHOLD       (25)

//Macros for Events triggered
#define TEMP_RCV_ID     (1)
#define LED_RCV_ID      (2)
#define TEMP_ALERT_ID   (3)

extern xSemaphoreHandle g_uartsem;
extern xSemaphoreHandle g_qsem;
extern xSemaphoreHandle g_temp;
extern xSemaphoreHandle g_led;
extern QueueHandle_t log_mq;
extern TaskHandle_t alert_id;

struct led
{
    TickType_t current_time;
    uint32_t toggle_cnt;
    char *name;
};

struct temp
{
    TickType_t current_time;
    float temp;
};

typedef struct
{
    uint8_t id;
    union
    {
        struct led led_data;
        struct temp temp_data;
    }data;
}event;


//Function Declarations
uint8_t temptask_init(void);
static void temp(void *pvParameters);
void temp_send(void);
float temp_read(void);

#endif /* INC_TEMP_H_ */
