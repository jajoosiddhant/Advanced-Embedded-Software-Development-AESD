/**
 * @file gpio.c
 * @Satya Mehta & Siddhant Jajoo
 * @brief 
 * Function to initialize GPIO and control USR LED on Beaglebone
 * @version 0.1
 * @date 2019-03-22
 * Some part referred from https://stackoverflow.com/questions/34261512/gpio-on-beaglebone-black
 * Tested using command lines.
 * @copyright Copyright (c) 2019
 * 
 */
#include "gpio.h"


/**
 * @brief Initialize GPIO pins 
 * export its directory in the system.
 * @param pin 
 * @return err_t 
 */
err_t gpio_init(uint8_t pin)
{
    FILE *fptr;
    fptr = fopen("/sys/class/gpio/export", "w");
    if (fptr == NULL)
    {
        error_log("ERROR: fopen(); in gpio_init() function", ERROR_DEBUG, P2);
    }
    fseek(fptr, 0, SEEK_SET); //set cursor to 0th position
    fprintf(fptr, "%d", pin);
    if (fclose(fptr) != 0)
    {
        error_log("ERROR: fclose(); in gpio_init() function", ERROR_DEBUG, P2);
    }
    return OK;
}


/**
 * @brief Control gpio pins value ON & OFF
 * Pass direction as first parameter
 * Pass value path as second paramter
 * Send 1, 0 as third parameter.
 * @param directory 
 * @param value 
 * @param onoff 
 * @return err_t 
 */
err_t gpio_ctrl(char *directory, char *value, uint8_t onoff)
{
    FILE *fptr;
    fptr = fopen(directory, "w");
    if (fptr == NULL)
    {
        error_log("ERROR: fopen(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    fseek(fptr, 0, SEEK_SET);   //set cursor to 0th position
    fprintf(fptr, "%s", "out"); //set gpio pin to output
    if (fclose(fptr) != 0)
    {
        error_log("ERROR: fclose(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    fptr = fopen(value, "w");
    if (fptr == NULL)
    {
        error_log("ERROR: fopen(value); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    fseek(fptr, 0, SEEK_SET);   //set cursor to 0th position
    fprintf(fptr, "%d", onoff); //set gpio pin value to high
    if (fclose(fptr) != 0)
    {
        error_log("ERROR: fcloses(value); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    return OK;
}

/**
 * @brief Can be used to set edge 
 * rising and falling.
 * 
 * @param directory 
 * @param level 
 * @return err_t 
 */

err_t gpio_edge(char *directory, char *level)
{
    FILE *fptr;
    fptr = fopen(directory, "w");
    if (fptr == NULL)
    {
        error_log("ERROR: fopen(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    fseek(fptr, 0, SEEK_SET);   //set cursor to 0th position
    fprintf(fptr, "%s", level); //set gpio pin to output
    if (fclose(fptr) != 0)
    {
        error_log("ERROR: fclose(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }  
    return OK;  
}

/**
 * @brief Set the direction of gpio 
 * in or out.
 * @param directory 
 * @param direction 
 * @return err_t 
 */
err_t gpio_dir(char *directory, char *direction)
{
    FILE *fptr;
    fptr = fopen(directory, "w");
    if (fptr == NULL)
    {
        error_log("ERROR: fopen(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }
    fseek(fptr, 0, SEEK_SET);   //set cursor to 0th position
    fprintf(fptr, "%s", direction); //set gpio pin to output
    if (fclose(fptr) != 0)
    {
        error_log("ERROR: fclose(directory); in gpio_ctrl() function", ERROR_DEBUG, P2);
    }    
    return OK;
}

/**
 * @brief Used to poll the gpio pin
 * 
 * @return uint8_t 
 */

uint8_t gpio_poll()
{
    struct pollfd pfd[2];
    int poll_out;
    pfd[0].fd = gpio_fd[0];
    pfd[0].events = POLLPRI;
    char val[5];
    poll_out = poll(pfd, 1, 10);
    if(poll_out == -1)
    {
        error_log("ERROR: pollng in gpio_poll() function", ERROR_DEBUG, P2);
    }
    if(pfd[0].revents & POLLPRI)
    {
        lseek(pfd[0].fd, 0, SEEK_SET);
        read(pfd[0].fd, val, 5);
        printf("Interrupt occured %s\n\n", val);
        return 0;
    }
    else
    {
        gpio_ctrl(GPIO54, GPIO54_V, 0);   
    }
    return OK;
}

/**
 * @brief Initialize gpio pin 60
 * 
 * @return err_t 
 */
err_t interrupt(void)
{
    gpio_init(60);
    gpio_dir(GPIO60, "in");
    gpio_edge(GPIO60_E, "both");
    gpio_fd[0] = open("/sys/class/gpio/gpio60/value", O_RDONLY);
    if(gpio_fd[0] == -1)
    {
        error_log("ERROR: open(gpio_fd[1]); in interrupt() function", ERROR_DEBUG, P2);
    }
    return OK;    
}