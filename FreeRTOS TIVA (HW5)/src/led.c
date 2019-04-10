/*
 * led.c
 *
 *  Created on: Apr 8, 2019
 *      Author: Siddhant Jajoo
 */

#include "inc/led.h"


/**
 * @brief This function creates LED Task
 * @return uint8_t
 */
uint8_t ledtask_init(void)
{

    if(xTaskCreate(led, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, NULL) != pdTRUE)
        {
            UARTprintf("Failed to create LED Task.\n");
            return 1;
        }

    return 0;
}

/**
 * @brief This function Configures the GPIO LEDs.
 * @return void
 */
void led_config(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
}


/**
 * @brief This function gets data related to LED in the struct event and sends it to the logger
 * task using message queues to print it via UART on the host console.
 * @return void
 */
void led_send(void)
{
    event info;

    info.id = LED_RCV_ID;
    info.data.led_data.name = "SIDDHANT JAJOO";
    info.data.led_data.toggle_cnt = cnt;
    info.data.led_data.current_time = xTaskGetTickCount();

    xSemaphoreTake(g_qsem,portMAX_DELAY);
    if(xQueueSend(log_mq, &info, portMAX_DELAY) != pdTRUE)
    {
//        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("LED Data Send failed.\n");
//        xSemaphoreGive(g_uartsem);
    }
    xSemaphoreGive(g_qsem);

}


/**
 * @brief Callback function for LED Task.
 * @return void
 */
static void led(void *pvParameters)
{
    uint8_t led0 = GPIO_PIN_0;
    uint8_t led1 = 0x00;

    while(1)
    {
        xSemaphoreTake(g_led,portMAX_DELAY);

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, led0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, led1);

        //Incrementing toggle count
        cnt++;

        //Changing LED pins status
        led0 ^= (GPIO_PIN_0);
        led1 ^= (GPIO_PIN_1);

        led_send();

        // xSemaphoreTake(g_uartsem,portMAX_DELAY);
        // UARTprintf("Welcome to LED Task.\n");
        // xSemaphoreGive(g_uartsem);
    }
}
