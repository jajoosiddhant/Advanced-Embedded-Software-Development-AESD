#ifndef _TIMER_H
#define _TIMER_H

#include "main.h"

#define TEMP_INTERVAL_SEC   (2)
#define TEMP_INTERVAL_NSEC  (0)
#define LIGHT_INTERVAL_SEC  (3)
#define LIGHT_INTERVAL_NSEC (0)
#define HB_INTERVAL_SEC (10)
#define HB_INTERVAL_NSEC (0)

err_t timer_init(uint8_t timer_number);
void timer_handler(union sigval sv);



#endif
