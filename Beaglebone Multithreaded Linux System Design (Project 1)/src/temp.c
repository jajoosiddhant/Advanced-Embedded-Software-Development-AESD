/**
 * @file temp.c
 * @author Satya Mehta & Siddhant Jajoo
 * @brief Functions and variables related to Temperature Sensor 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "temp.h"
#include "gpio.h"

/**
 * @brief Read Temperature
 * 
 * @param temp_unit - Specify 0, 1, 2 to change the unit.
 * @param id - Enter Id for the logger
 * @return sensor_struct 
 */
sensor_struct read_temp_data(uint8_t temp_unit, uint8_t id)
{
    uint16_t temp;
    char temp_buff[2];
    int temp_data[2];
    sensor_struct read_data;
    write_pointer(TEMP_REG); //select temperature register

    if (read(i2c_open, temp_buff, 2) != 2)
    {
        error_log("ERROR: read(); in read_temp_data() function", ERROR_DEBUG, P2);
    }
    temp_data[0] = (int)temp_buff[0]; //storing MSB
    temp_data[1] = (int)temp_buff[1]; //storing LSB

    read_data.id = id;
    if (clock_gettime(CLOCK_REALTIME, &read_data.sensor_data.temp_data.data_time))
    {
        error_log("ERROR: clock_gettime(); in read_temp_data() function", ERROR_DEBUG, P2);
    }
    temp = ((((uint16_t)temp_data[0]) << 4) | (temp_data[1] >> 4)) & 0x0FFF;
    float final_temp;
    if (temp_data[0] & 0x80)
    {
        final_temp = ((~temp)+1) * 0.0625;
    }
    else
    {
        final_temp = 0.0625 * temp;
    }
    read_data.sensor_data.temp_data.temp_c = final_temp;
    //read_data.sensor_data.temp_data.temp_c = (float)((temp_data[0] << 8 | temp_data[1]) >> 4) * 0.0625; //referred calculations from http://bildr.org/2011/01/tmp102-arduino/
    if (temp_unit == 1)
    {
        read_data.sensor_data.temp_data.temp_c = read_data.sensor_data.temp_data.temp_c + 273; //Kelvin Computations
    }
    else if (temp_unit == 2)
    {
        read_data.sensor_data.temp_data.temp_c = ((9.0 / 5.0) * (read_data.sensor_data.temp_data.temp_c)) + 32; //Farhenheit calculations
    }
    return read_data;
}

/**
 * @brief Can be used to read any temperature register
 * Thigh, Tlow and Temperature
 * 
 * @param reg - Pass address of the register which is required to be read.
 * @return uint16_t 
 */

uint16_t read_temp_reg(uint8_t reg)
{
    int temp;
    uint8_t data[2];
    uint16_t final;
    write_pointer(reg);
    if (read(i2c_open, &data, 2) != 2)
    {
        error_log("ERROR: read(); in read_temp_reg() function", ERROR_DEBUG, P2);
    }
    final = (data[0] << 4) | (data[1] >> 4);
    write_pointer(TEMP_REG);
    return final;
}
/**
 * @brief Used to read config register
 * 
 * @return uint16_t 
 */

uint16_t read_config()
{
    int rc;
    uint16_t data;
    write_pointer(CONFIG_REG);
    rc = read(i2c_open, &data, 2);
    printf("No of bytes read %d\n\n", rc);
    // printf("READ LSB %x", data[1]);
    return data;
}
/**
 * @brief Set or clear the shutdown mode
 * 
 * @param onoff 
 * @return err_t 
 */

err_t shutdown_mode(uint8_t onoff)
{

    int rc;
    uint16_t config = read_config();
    config |= (uint16_t)(onoff << SHUTDOWN_MASK);
    write_config(config);
    printf("RC: %d\n", rc);
    write_pointer(TEMP_REG);
    return 0;
}

/**
 * @brief Read resolution bits
 * 
 * @return uint8_t 
 */
uint8_t read_resolution(void)
{
    uint16_t config = read_config();
    uint8_t data = config >> 8;
    data = data & 0x60; // masking with R1,R2 bit
    return data; 
}

/**
 * @brief Write to the config register
 * 
 * @param data - Specify the data which is to be written to cnfig register
 * @return err_t 
 */
err_t write_config(uint16_t data)
{
    int temp, rc;
    uint8_t buff[3];
    temp = data >> 8;
    buff[0] = CONFIG_REG;
    buff[1] = temp;
    buff[2] = (uint8_t)data;
    write_pointer(CONFIG_REG);
    if ((rc = write(i2c_open, buff, 3)) != 3)
    {
        error_log("ERROR: write(); in write_config function", ERROR_DEBUG, P2);
        perror("In write config");
    }
    printf("Written %x", buff[1] | buff[2]);
}

/**
 * @brief Write to the tlow register
 * inside the sensor
 * @param data 
 * @return err_t 
 */

err_t write_tlow(uint16_t data)
{
    int rc;
    int temp;
    data = data / 0.0625;
    write_pointer(TLOW_REG);
    read_buff[1] = data >> 4;
    read_buff[2] = data << 4;
    uint8_t buff[3] = {TLOW_REG, read_buff[1], read_buff[2]};
    if ((rc = write(i2c_open, &buff, 3)) != 3)
    {
        error_log("ERROR: write(); in write_thigh() function", ERROR_DEBUG, P2);
    }
    write_pointer(TEMP_REG);
}


/**
 * @brief Set the fault bits
 * 
 * @param data - Pass data which is written to the fault bits
 * @return uint16_t 
 */

uint16_t set_fault_bits(uint8_t data)
{
    int rc;
    int buff[3];
    uint16_t config = read_config();
    config = config | (data << FAULT_MASK);
    write_config(config);
    write_pointer(TEMP_REG);
}

/**
 * @brief Read the fault bits
 * 
 * @return uint8_t 
 */

uint8_t read_fault_bits(void)
{
    uint16_t config = read_config();
    uint8_t data = config >> 3;
    return data;
}

/**
 * @brief Read extended bits.
 * 
 * @return uint8_t 
 */

uint8_t read_extended(void)
{
    int rc;
    int data;
    uint16_t config = read_config();
    data = (config >> 8) & 0x10;
    data = data >> 4;
    return data;
}

/**
 * @brief Set the extended bit to 1 or 0
 * 
 * @param onoff - Pass 1 to set and 0 to reset
 */

void set_extended(uint8_t onoff)
{
    int rc;
    uint8_t temp;
    uint16_t config = read_config();
    if(onoff == 1)
    {
        config |= 0x1000;
    }
    else
    {
        config |= 0x0000;
    }
}

/**
 * @brief Read conversion bits from the config register
 * 
 * @return uint8_t 
 */
uint8_t read_conversion(void)
{
    int rc;
    int data;
    uint16_t config = read_config();
    data = config >> 14;
    return data;
}

/**
 * @brief Set the conversion bits in the config register
 * 
 * @param data - Pass the data to set/reset the conversion bits
 */

void set_conversion(uint8_t data)
{
    uint16_t config = read_config();
    data = data << CONVERSION;
    config |= data;
    write_config(config);
}

/**
 * @brief Write to thigh register
 *inside the sensor.
 * @param data - Pass the data to be written to the thigh register
 * @return err_t 
 */
err_t write_thigh(uint16_t data)
{
    int rc;
    int temp;
    data = data / 0.0625;
    write_pointer(THIGH_REG);
    read_buff[1] = data >> 4;
    read_buff[2] = data << 4;
    uint8_t buff[3] = {THIGH_REG, read_buff[1], read_buff[2]};
    if ((rc = write(i2c_open, &buff, 3)) != 3)
    {
        error_log("ERROR: write(); in write_thigh() function", ERROR_DEBUG, P2);
    }
    write_pointer(TEMP_REG);
}

/**
 * @brief Write to pointer register avaialable inside the sensor.
 * 
 * @param reg - Pass the register address
 * @return err_t 
 */
err_t write_pointer(uint8_t reg)
{
    int rc;
    if (ioctl(i2c_open, I2C_SLAVE, TEMP_ADDR) < 0)
    {
        error_log("ERROR: ioctl(); in write_pointer() function", ERROR_DEBUG, P2);
    }
    if ((rc = write(i2c_open, &reg, 1)) != 1)
    {
        error_log("ERROR: write(); in write_pointer() function", ERROR_DEBUG, P2);
    }
    return 0;
}
