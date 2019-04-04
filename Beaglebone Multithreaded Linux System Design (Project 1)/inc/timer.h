/**
 * @file timer.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file for timer.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"

//Timer intervals
#define TEMP_INTERVAL_SEC   (2)
#define TEMP_INTERVAL_NSEC  (0)
#define LIGHT_INTERVAL_SEC  (3)
#define LIGHT_INTERVAL_NSEC (0)
#define HB_INTERVAL_SEC (10)
#define HB_INTERVAL_NSEC (0)

//Timer Handle declarations
timer_t timeout_temp;
timer_t timeout_light;
timer_t timeout_hb;

//Function declarations
err_t timer_init(uint8_t timer_number);
void timer_handler(union sigval sv);
err_t timer_del(void);

#endif
