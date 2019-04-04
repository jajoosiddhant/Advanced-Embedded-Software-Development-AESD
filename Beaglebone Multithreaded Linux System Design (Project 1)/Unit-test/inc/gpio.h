#ifndef _GPIO_H
#define _GPIO_H
#include "main.h"

#define LED1 53
#define LED2 54
#define LED3 55
#define LED4 56

#define GPIO53  "/sys/class/gpio/gpio53/direction"
#define GPIO54  "/sys/class/gpio/gpio54/direction"
#define GPIO55  "/sys/class/gpio/gpio55/direction"
#define GPIO56  "/sys/class/gpio/gpio53/direction"

#define GPIO53_V  "/sys/class/gpio/gpio53/value"
#define GPIO54_V  "/sys/class/gpio/gpio54/value"
#define GPIO55_V  "/sys/class/gpio/gpio55/value"
#define GPIO56_V  "/sys/class/gpio/gpio53/value"

err_t gpio_init(uint8_t);
err_t gpio_ctrl(char *, char *, uint8_t);
#endif