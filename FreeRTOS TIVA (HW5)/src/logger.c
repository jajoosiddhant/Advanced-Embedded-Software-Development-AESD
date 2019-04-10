/*
 * logger.c
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#include "inc/logger.h"


/**
 * @brief This function creates logger Task
 * @return uint8_t
 */
uint8_t loggertask_init(void)
{
    if(xTaskCreate(logger, (const portCHAR *)"logger", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, NULL) != pdTRUE)
    {
        UARTprintf("Failed to create Logger Task.\n");
        return(1);
    }

    return 0;
}

/**
 * @brief This function receives the data sent from different tasks and
 * prints it via UART on the host console.
 * @return void
 */
void log(void)
{
    uint32_t temp_var;
    event info;

    if(xQueueReceive(log_mq, (void *)&info, portMAX_DELAY) == pdFALSE)
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("Receive from Queue failed.\n");
        xSemaphoreGive(g_uartsem);
    }

    switch(info.id)
    {
    case TEMP_RCV_ID:
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("/**TEMPERATURE DATA**/\n");
        UARTprintf("Timestamp: %d milliseconds\n", info.data.temp_data.current_time);
        UARTprintf("%d\n",(uint8_t)info.data.temp_data.temp);
        UARTprintf("%d\n", (uint32_t)(info.data.temp_data.temp * 10000));
        temp_var = (uint32_t)(info.data.temp_data.temp * 10000) - (uint8_t)info.data.temp_data.temp * 10000;
        UARTprintf("Temperature Recorded: %d.%d", (uint8_t)info.data.temp_data.temp, temp_var);
        UARTprintf("\n*****************************************************\n\n");
        xSemaphoreGive(g_uartsem);
        break;
    }

    case LED_RCV_ID:
    {
//        xSemaphoreTake(g_uartsem,portMAX_DELAY);
//        UARTprintf("/**LED DATA**/\n");
//        UARTprintf("Timestamp: %d milliseconds\n", info.data.led_data.current_time);
//        UARTprintf("Name: %s\n", info.data.led_data.name);
//        UARTprintf("LED 1 Toggle Count: %d\n", info.data.led_data.toggle_cnt);
//        UARTprintf("LED 2 Toggle Count: %d\n", info.data.led_data.toggle_cnt - 1);
//        UARTprintf("\n*****************************************************\n\n");
//        xSemaphoreGive(g_uartsem);
        break;
    }

    case TEMP_ALERT_ID:
    {
        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("/**ALERT!!! ALERT!!! ALERT!!!**/\n");
        UARTprintf("Temperature has crossed the Threshold.\n");
        UARTprintf("Timestamp: %d milliseconds\n", info.data.temp_data.current_time);
//        UARTprintf("%d\n",(uint8_t)info.data.temp_data.temp);
//        UARTprintf("%d\n", (uint32_t)(info.data.temp_data.temp * 10000));
        temp_var = (uint32_t)(info.data.temp_data.temp * 10000) - (uint8_t)info.data.temp_data.temp * 10000;
        UARTprintf("Temperature Recorded: %d.%d", (uint8_t)info.data.temp_data.temp, temp_var);
        UARTprintf("\n*****************************************************\n\n");
        xSemaphoreGive(g_uartsem);
        break;
    }


    }
}


/**
 * @brief Callback function for Logger Task.
 * @return void
 */
static void logger(void *pvParameters)
{
    while(1)
    {
        //Logs all the messages it receives
        log();

    }


}

