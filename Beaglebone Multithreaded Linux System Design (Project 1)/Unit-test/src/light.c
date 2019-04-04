/**
 * @file light.c
 * @author Satya Mehta and Siddhant Jajoo
 * @brief Functions and initializations for Light sensor
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "light.h"

char read_buff;

/**
 * @brief read_light_data() reads lux data from the sensor.
 * Acquires the bus, sets the control register, powers up the sensor and calls lux_data() function
 * @param id 
 * @return sensor_struct 
 */
float read_light_data(uint8_t id)
{
    sensor_struct read_data;
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0)
    {
        perror("ERROR");
    }
    write_command(CNTRL_REG);
    char buff = 0x03; //To power up the sensor
    if (write(i2c_open, &buff, 1) != 1)
    {
       perror("ERROR");
    }
    read_data.id = id;
    //read_data.sensor_data.light_data.light = lux_data(); //Get lux data from the light sensor.
    return lux_data();
}

/**
 * @brief Read light identification register from the sensor
 * 
 * @return err_t 
 */
uint8_t light_id(void)
{
    printf("Inside light id\n\n");
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0)
    {
        perror("ERROR");
    }
    write_command(CNTRL_REG);
    char buff = 0x03;
    if (write(i2c_open, &buff, 1) != 1)
    {
        perror("ERROR");
    }
    write_command(ID_REG);
    uint8_t id;
    if (read(i2c_open, &id, 1) != 1)
    {
        perror("ERROR");
    }
    printf("Light Id: %x\n", (int)id);
    return id;
}

/**
 * @brief Used to set the command register in the sensor
 * 
 * @param reg_addr 
 * @return err_t 
 */
err_t write_command(uint8_t reg_addr)
{
    char buff = COMMAND_MASK | reg_addr;
    if (write(i2c_open, &buff, 1) != 1)
    {
        perror("ERROR");
    }
    return 0;
}
/**
 * @brief Read ADC Channel 0 of the sensor
 * 
 * @return uint16_t 
 */

uint16_t ADC_CH0(void)
{
    uint8_t lsb;
    uint16_t ch0, msb;
    write_command(ADC0_L);
    if (read(i2c_open, &lsb, 1) != 1)
    {
        perror("ERROR");
    }
    write_command(ADC0_H);
    if (read(i2c_open, &msb, 1) != 1)
    {
        perror("ERROR");
    }
    msb = msb << 8;
    ch0 = msb | lsb;
    return ch0;
}
/**
 * @brief Read ADC_CH0 data independently
 * 
 * @return uint16_t 
 */

uint16_t read_adc0(void)
{
    uint16_t data;
    if ((i2c_open = open(I2C_BUS, O_RDWR)) < 0)
    {
        perror("ERROR");
    }
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0)
    {
        perror("ERROR");
    }
    data = ADC_CH0();
    return data;
}
/**
 * @brief Read ADC channel 1 from the sensor.
 * 
 * @return uint16_t 
 */

uint16_t ADC_CH1(void)
{
    uint8_t lsb;
    uint16_t ch1, msb;
    write_command(ADC1_L);
    if (read(i2c_open, &lsb, 1) != 1)
    {
        perror("ERROR");
    }
    write_command(ADC1_H);
    if (read(i2c_open, &msb, 1) != 1)
    {
        perror("ERROR");
    }
    msb = msb << 8;
    ch1 = msb | lsb;
    return ch1;
}

/**
 * @brief Lux Data calculations
 * Reads ADC_CH0 and ADC_CH1 data and does the computations.
 * 
 * 
 * @return float 
 */

float lux_data(void)
{
    uint16_t adc0, adc1;
    adc0 = ADC_CH0();
    adc1 = ADC_CH1();
    float final_adc = (float)(adc1 / adc0);
    float lux_data;
    if (0 < final_adc <= 0.50)
    {
        lux_data = (0.0304 * adc0) - ((0.062 * adc0) * (pow((adc1 / adc0), 1.4)));
    }
    else if (0.5 < final_adc && final_adc <= 0.61)
    {
        lux_data = (0.0224 * adc0) - (0.031 * adc1);
    }
    else if (0.61 < final_adc && final_adc <= 0.8)
    {
        lux_data = (0.0128 * adc0) - (0.0153 * adc1);
    }
    else if (0.8 < final_adc && final_adc <= 1.3)
    {
        lux_data = (0.00146 * adc0) - (0.00112 * adc1);
    }
    else if (final_adc > 1.30)
    {
        lux_data = 0;
    }
    return lux_data;
    //return adc0;
}

/**
 * @brief Writes to interrupt threshold register. 
 * Takes data and interrupt threshold register as parameters.
 * 
 * @param data 
 * @param reg 
 * @return err_t 
 */

err_t write_int_th(uint16_t data, uint8_t reg)
{
    uint8_t temp;
    if (reg == 0)
    {
        write_command(INT_L_L);
        temp = data & 0x00FF;
        if (write(i2c_open, &temp, 1) != 1)
        {
            perror("ERROR");
        }
        write_command(INT_L_H);
        temp = data >> 8;
        if (write(i2c_open, &temp, 1) != 1)
        {
            perror("ERROR");
        }
    }
    if (reg == 1)
    {
        write_command(INT_H_L);
        temp = data & 0x00FF;
        if (write(i2c_open, &temp, 1) != 1)
        {
            perror("ERROR");
        }
        write_command(INT_H_H);
        temp = data >> 8;
        if (write(i2c_open, &temp, 1) != 1)
        {
            perror("ERROR");
        }
    }
    return 0;
}