//*****************************************************************************
//
// senshub_iot.c - Example to publish SensorHub BoosterPack data to the cloud.
//
// Copyright (c) 2013-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"
#include "inc/led.h"
#include "inc/logger.h"
#include "inc/temp.h"

#define TEMP_INTERVAL   (120000000)     //Corresponds to one second
#define LED_INTERVAL    (12000000)      //Corresponds to 100 milliseconds
#define MQ_SIZE         (10)            // Size of the message queue


// The mutex that protects concurrent access of UART from multiple tasks.
xSemaphoreHandle g_uartsem;
xSemaphoreHandle g_qsem;

// Global variable to hold the system clock speed.
uint32_t g_ui32SysClock;

//Semaphores for temperature and led.
xSemaphoreHandle g_temp;
xSemaphoreHandle g_led;

//Message Queue Handle
QueueHandle_t log_mq;

//Task ID
TaskHandle_t alert_id;

// Counter value used by the FreeRTOS run time stats feature.
// http://www.freertos.org/rtos-run-time-stats.html
volatile unsigned long g_vulRunTimeStatsCountValue;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif


/**
 * brief  Interrupt handler for Timer0A.
 * This function will be called periodically on the expiration of Timer0A. It
 * release the semaphore corresponding to the temperature task in order to take
 * readings at 1 Hz.
 * @return void
 */
void temp_timer0handler(void)
{
    //Disable all interrupts
    ROM_IntMasterDisable();

    // Clear the timer interrupt.
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Release the Semaphore for the Temperature Task
    xSemaphoreGive(g_temp);

    //Enable all interrupts
    ROM_IntMasterEnable();
}


/**
 * brief  Interrupt handler for Timer1A.
 * This function will be called periodically on the expiration of Timer1A. It
 * release the semaphore corresponding to the LED task in order to blink the LEDs
 * at a frequency of 10 Hz.
 * @return void
 */
void led_timer0handler(void)
{
    //Disable all interrupts
    ROM_IntMasterDisable();

    // Clear the timer interrupt.
    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //Release the Semaphore for the LED Task
    xSemaphoreGive(g_led);

    //Enable all interrupts
    ROM_IntMasterEnable();
}



/**
 * @brief Callback function for Alert Task
 * @return void
 */
void alert(void *pvParameters)
{
    while(1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);

        xSemaphoreTake(g_uartsem,portMAX_DELAY);
        UARTprintf("/**ALERT!!! ALERT!!! ALERT!!!**/\n");
        xSemaphoreGive(g_uartsem);
    }
}



/**
 * @brief This function creates Alert Task
 * @return uint8_t
 */
uint8_t alerttask_init(void)
{
    if(xTaskCreate(alert, (const portCHAR *)"alert", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY, &alert_id) != pdTRUE)
    {
        UARTprintf("Failed to create Alert Task.\n");
        return(1);
    }
    return 0;
}



//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


/**
 * brief Function to create and enable timers for Temperature Sensors and LED Toggling.
 * TIMER0 corresponds to Temperature timer.
 * TIMER1 corresponds to LED Timer
 * @return void
 */
void timer_config()
{

    // Enable the peripherals for Timers.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    // Configure the 32-bit periodic timer.
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, TEMP_INTERVAL);
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, LED_INTERVAL);

    // Setup the interrupts for the timer timeout.
    ROM_IntEnable(INT_TIMER0A);
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);
}


/**
 * @brief Function creates Semaphores for Temperature and LED Tasks.
 * @return
 */
void sem_create(void)
{
    //Creating Temperature Semaphore
    g_temp = xSemaphoreCreateBinary();
    if(g_temp == NULL)
    {
        UARTprintf("\n\nTemperature Semaphore not created.\n");
        exit(EXIT_FAILURE);
    }

    //Creating LED Semaphore
    g_led = xSemaphoreCreateBinary();
    if(g_led == NULL)
    {
        UARTprintf("\n\nLED Semaphore not created.\n");
        exit(EXIT_FAILURE);
    }

}


/**
 * @brief Configure the UART and its pins.  This must be called before UARTprintf().
 * @return void
 */
void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

/**
 * @brief This function configures I2C
 * @return void
 */
void configure_i2c(void)
{
    //GPIO Peripheral Enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);

    // Wait for the Peripheral to be ready for programming
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)
            || !SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2));

    // Configure Pins for I2C7 Master Interface
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);

    // Initialize and Configure the Master Module
    I2CMasterInitExpClk(I2C2_BASE, g_ui32SysClock, false);
}



/**
 * @brief  Initialize FreeRTOS and start the initial set of tasks.
 * @return int
 */
int main(void)
{
    // Configure the system frequency.
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);


    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART();

    //Configuring I2C
    configure_i2c();

    //Configuring LEDs
    led_config();

    //Creating a message queue
    log_mq = xQueueCreate(MQ_SIZE, sizeof(event));
    {
        if( log_mq == NULL )
        {
            UARTprintf("\n\nMessage Queue not created.\n");
            exit(EXIT_FAILURE);
        }
    }

    //Creating Semaphores
    sem_create();

    //Creating Mutex for UART
    g_uartsem = xSemaphoreCreateMutex();
    if(g_uartsem == NULL)
    {
        UARTprintf("\n\nUART Mutex not created.\n");
        exit(EXIT_FAILURE);
    }

    g_qsem = xSemaphoreCreateMutex();
    if(g_qsem == NULL)
    {
        UARTprintf("\n\nQueue Mutex not created.\n");
        exit(EXIT_FAILURE);
    }

    UARTprintf("Welcome to Exercise 5 FreeRTOS AESD Demo!\n");

    // Create Temperature Task.
    if(temptask_init() != 0)
    {
        while(1)
        {
        }
    }

    // Create LED Task.
    if(ledtask_init() != 0)
    {
        while(1)
        {
        }
    }

    // Create Logger Task.
    if(loggertask_init() != 0)
    {
        while(1)
        {
        }
    }

//     Create Alert Task.
    if(alerttask_init() != 0)
    {
        while(1)
        {
        }
    }

    //Creates and Enables Timers
    timer_config();

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, print an error and loop forever.
    UARTprintf("RTOS scheduler returned unexpectedly.\n");
    while(1)
    {
        // Do Nothing.
    }
}
