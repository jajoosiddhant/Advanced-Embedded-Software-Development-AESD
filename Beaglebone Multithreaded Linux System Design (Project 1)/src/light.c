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

int read_buff;

/**
 * @brief read_light_data() reads lux data from the sensor.
 * Acquires the bus, sets the control register, powers up the sensor and calls lux_data() function
 * @param id - Strores the identification of event into the structure
 * @return sensor_struct 
 */
sensor_struct read_light_data(uint8_t id)
{
    sensor_struct read_data;
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0) 
    {
        error_log("ERROR: ioctl(); in read_light_data() function", ERROR_DEBUG, P2);
    }
    write_command(CNTRL_REG);
    char buff = 0x03; //To power up the sensor
    if(write(i2c_open,&buff,1) != 1) 
    {
       error_log("ERROR: write(); in read_light_data() function", ERROR_DEBUG, P2);
    }
    read_data.id = id;
    if (clock_gettime(CLOCK_REALTIME, &read_data.sensor_data.light_data.data_time))
    {
        error_log("ERROR: clock_gettime(); in read_light_data() function", ERROR_DEBUG, P2);
    }
    read_data.sensor_data.light_data.light = lux_data();  //Get lux data from the light sensor.
    if(read_data.sensor_data.light_data.light < LIGHT_TH)
    {
        read_data.sensor_data.light_data.light_state = DARK;
    }
    else
    {
        read_data.sensor_data.light_data.light_state = LIGHT;
    }
    return read_data;    
}


/**
 * @brief Read light identification register from the sensor
 * 
 * @return uint8_t 
 */
uint8_t light_id(void)
{
    printf("Inside light id\n\n");
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0) 
    {
        error_log("ERROR: ioctl(); in light_id() function", ERROR_DEBUG, P2);
    }
    write_command(CNTRL_REG);
    char buff = 0x03;
    if(write(i2c_open,&buff,1) != 1) 
    {
       error_log("ERROR: write(); in light_id() function", ERROR_DEBUG, P2);
    }
    write_command(ID_REG);
    uint8_t id;
    if (read(i2c_open,&id,1) != 1) 
    {
       error_log("ERROR: read(); in light_id() function", ERROR_DEBUG, P2);
    }
    return id;
}

/**
 * @brief Used to set the command register in the sensor
 * 
 * @param reg_addr - Pass the register address to write in the command register
 * @return err_t 
 */
err_t write_command(uint8_t reg_addr)
{
    char buff = COMMAND_MASK|reg_addr;
    if(write(i2c_open,&buff,1) != 1) 
    {
       error_log("ERROR: write(); in write_command() function", ERROR_DEBUG, P2);
    }
    return OK;

}
/**
 * @brief Read ADC Channel 0 of the sensor
 * 
 * @return uint16_t 
 */

uint16_t ADC_CH0(void)
{
    uint8_t lsb;
    uint16_t ch0,msb;
    write_command(ADC0_L);
    if (read(i2c_open,&lsb,1) != 1) 
    {
       error_log("ERROR: read(lsb); in ADC_CH0() function", ERROR_DEBUG, P2);
    }
    write_command(ADC0_H);
    if (read(i2c_open,&msb,1) != 1) 
    {
       error_log("ERROR: read(msb); in ADC_CH0() function", ERROR_DEBUG, P2);
    }
    msb = msb << 8;
    ch0 = msb|lsb;
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
    if((i2c_open = open(I2C_BUS, O_RDWR)) < 0)
    {
        error_log("ERROR: open(); in light_id() function", ERROR_DEBUG, P2);
    }
    if (ioctl(i2c_open, I2C_SLAVE, LIGHT_ADDR) < 0) 
    {
        error_log("ERROR: ioctl(); in light_id() function", ERROR_DEBUG, P2);
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
    uint16_t ch1,msb;
    write_command(ADC1_L);
    if (read(i2c_open,&lsb,1) != 1) 
    {
       error_log("ERROR: read(lsb); in ADC_CH1() function", ERROR_DEBUG, P2);
    }
    write_command(ADC1_H);
    if (read(i2c_open,&msb,1) != 1) 
    {
       error_log("ERROR: read(msb); in ADC_CH1() function", ERROR_DEBUG, P2);
    }
    msb = msb << 8;
    ch1 = msb|lsb;
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
    float final_adc = (float)(adc1/adc0);
    float lux_data;
    if( 0 < final_adc <= 0.50)
    {
        lux_data = (0.0304 * adc0) - ((0.062 * adc0) * (pow((adc1/adc0), 1.4)));
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
        lux_data = (0.00146 * adc0) - (0.00112*adc1);
    }
    else if(final_adc > 1.30)
    {
        lux_data = 0;
    }
    return lux_data;
}


/**
 * @brief Can be used to read any register inside the sensor
 * 
 * @param reg 
 * @return err_t 
 */
err_t read_light_reg(uint8_t reg)
{
    write_command(reg);
    if (read(i2c_open, &read_buff,1) != 1) 
    {
       error_log("ERROR: read(); in read_light_reg() function", ERROR_DEBUG, P2);
    }
    return 0;

}
/**
 * @brief Writes timing register
 * 
 * @param data - Pass the data which is required to write.
 * @return err_t 
 */

err_t write_timing_reg(uint8_t data)
{
    write_command(TIMING_REG);
    read_light_reg(TIMING_REG);
    data = data | read_buff;
    if(write(i2c_open,&data,1) != 1) 
    {
       error_log("ERROR: write(); in write_timing_reg() function", ERROR_DEBUG, P2);
    }
    return OK;
}

/**
 * @brief Writes to interrupt control register
 * 
 * @param data 
 * @return err_t 
 */
err_t write_int_ctrl(uint8_t data)
{
    write_command(INT_CTRL);
    read_light_reg(INT_CTRL);
    data = data | read_buff;
    if(write(i2c_open,&data,1) != 1) 
    {
      error_log("ERROR: write(); in write_int_reg() function", ERROR_DEBUG, P2);
    }
    return OK;

}

/**
 * @brief Can be used to read the interrupt threshold 
 * register.
 * @param reg  - Reg specifies MSB or LSB
 * @return uint16_t 
 */

uint16_t read_int_th(uint8_t reg)
{
    uint16_t data;
    uint8_t temp;
    if(reg == 0)
    {
        read_light_reg(INT_L_L);
        temp = read_buff;
        read_light_reg(INT_L_H);
        data = read_buff;
        data = data << 8;
        data = data | temp;

    }
    if(reg == 1)
    {
        read_light_reg(INT_H_L);
        temp = read_buff;
        read_light_reg(INT_H_H);
        data = read_buff;
        data = data << 8;
        data = data | temp;

    }
    return data;
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
    if(reg == 0)
    {
        write_command(INT_L_L);
        temp = data & 0x00FF;
        if(write(i2c_open,&temp,1) != 1) 
        {
            error_log("ERROR: write(); reg = 0; in write_int_th() function", ERROR_DEBUG, P2);
        }
        write_command(INT_L_H);
        temp = data >> 8;
        if(write(i2c_open,&temp,1) != 1) 
        {
            error_log("ERROR: write(); reg = 0; in write_int_th() function", ERROR_DEBUG, P2);
        }
    }
    if(reg == 1)
    {
        write_command(INT_H_L);
        temp = data & 0x00FF;
        if(write(i2c_open,&temp,1) != 1) 
        {
           error_log("ERROR: write(); reg = 1; in write_int_th() function", ERROR_DEBUG, P2);
        }
        write_command(INT_H_H);
        temp = data >> 8;
        if(write(i2c_open,&temp,1) != 1) 
        {
            error_log("ERROR: write(); reg = 1; in write_int_th() function", ERROR_DEBUG, P2);
        }
    }
    return 0;
}