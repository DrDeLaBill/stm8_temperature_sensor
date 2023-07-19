#ifndef __I2C_H__
#define __I2C_H__


#include <stdint.h>


typedef enum {
    I2C_SUCCESS = 0,
    I2C_TIMEOUT,
    I2C_ERROR
} i2c_status_t;


void i2c_master_init(uint32_t f_master_hz, uint32_t f_i2c_hz);
i2c_status_t i2c_wr_reg(uint8_t address, uint8_t reg_addr, char* data, uint8_t length);
i2c_status_t i2c_rd_reg(uint8_t address, uint8_t reg_addr, uint8_t* data, uint8_t length);


#endif
