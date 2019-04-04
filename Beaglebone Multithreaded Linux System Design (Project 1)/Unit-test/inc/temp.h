/**
 * @file temp.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file of temp.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _TEMP_H
#define _TEMP_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "main.h"
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define TEMP_ADDR (0x48)
#define TEMP_REG (0)
#define CONFIG_REG (0x01)
#define THIGH_REG (3)
#define TLOW_REG (2)

#define CONFIG_DEFAULT (0x60A0)

#define FAULT_MASK (11) //3
#define EM_MASK    (4) //4
#define CONVERSION (6) //14
#define SHUTDOWN_MASK (8)
#define RESO_MASK (13)
#define AL_MASK (1)



uint8_t read_buff[3];

//Function Declarations
//err_t i2c_init(void);
uint16_t read_temp_reg(uint8_t);
sensor_struct read_temp_data(uint8_t temp_unit, uint8_t);
err_t write_pointer(uint8_t);
err_t shutdown_mode(uint8_t);
uint16_t set_fault_bits(uint8_t data);
uint8_t read_fault_bits(void);
void set_extended(uint8_t onoff);
void set_conversion(uint8_t data);
uint8_t read_conversion(void);
err_t write_tlow(uint16_t);
err_t write_thigh(uint16_t);
err_t write_config(uint16_t);
uint16_t read_config(void);
#endif