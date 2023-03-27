#ifndef _I2C_H_
#define _I2C_H_


#include "stm8s.h"


#define F_MASTER_MHZ    16UL
#define F_MASTER_HZ     16000000UL


//Результат выполнения операции с i2c
typedef enum {
    I2C_SUCCESS = 0,
    I2C_TIMEOUT,
    I2C_ERROR
} t_i2c_status;

// Инициализация I2C интерфейса                                   
extern void i2c_master_init(unsigned long f_master_hz, unsigned long f_i2c_hz);

// Запись регистра slave-устройства
extern t_i2c_status  i2c_wr_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, 
                               unsigned char length);

// Чтение регистра slave-устройства
extern t_i2c_status  i2c_rd_reg(unsigned char address, 
                               unsigned char reg_addr,
                               char * data, 
                               unsigned char length);


#endif