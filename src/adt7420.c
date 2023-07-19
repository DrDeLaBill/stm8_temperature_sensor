#include "adt7420.h"

#include <stdbool.h>

#include "stm8s.h"
#include "i2c.h"
#include "main.h"
#include "utils.h"
#include "mb.h"
#include "mb-table.h"


#define CONFIG_SLEEP (uint8_t)0b01100000
#define CONFIG_READ  (uint8_t)0b01000000
#define STATUS_RDY   (uint8_t)0b10000000


uint32_t adt7420_delay = 0;
bool adt_init_success = true;


void adt7420_proccess()
{
  if (!adt_init_success) {
    mb_table_write(TABLE_Holding_Registers, TEMPERATURE_REGISTER, 0xFFFF);
    return;
  }

  if ((uint32_t)abs_dif(Global_time, adt7420_delay) < ADT7420_DELAY) {
    return;
  }

  adt7420_delay = Global_time;
  int16_t tempr = 0;
  t_i2c_status status = adt7420_get_temperature(&tempr);
  int16_t res = 0;

  if (status != I2C_SUCCESS) {
    res = 0xFFFF;
  } else {
    uint16_t buf = (((uint16_t)((tempr & 0xFF00) >> 8) & 0x7F) << 5) | ((uint8_t)(tempr & 0x00FF) >> 3);
    res = (int16_t)((buf * 10) / 16);
    bool sign = 0x80 & ((tempr & 0xFF00) >> 8);
    res *= (sign ? -1 : 1);
  }

  mb_table_write(TABLE_Holding_Registers, TEMPERATURE_REGISTER, res);
}

t_i2c_status adt7420_init()
{
  i2c_master_init(get_clock_freq(), F_I2C_HZ);
  
  t_i2c_status status = adt7420_available();
  if (status != I2C_SUCCESS) {
    goto do_error;
  }

  uint8_t conf = CONFIG_SLEEP;
  status = i2c_wr_reg(I2C_ADT7420_ADDR, ADT7420_CONFIGURATION, &conf, 1);
  if (status != I2C_SUCCESS) {
    goto do_error;
  }

  goto do_exit;

do_error:
  adt_init_success = false;
  goto do_exit;

do_exit:
  return status;
}

t_i2c_status adt7420_available() 
{
  uint8_t resp = 0;
  t_i2c_status status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_ID, &resp, 1);

  if ((resp & 0xF0) != 0xC0) {
    return I2C_ERROR;
  }

  return status;
}

//******************************************************************************
//Чтение температуры из регистров ADT7420
//******************************************************************************
t_i2c_status adt7420_get_temperature(int16_t* value)
{
  //Чтение регистров adt7420
  uint8_t resp_h = 0;
  uint8_t resp_l = 0;

  t_i2c_status status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_MOST_SIGNIFICANT_BYTE, &resp_h, 1);
  if (status == I2C_SUCCESS) {
    *value |= ((uint16_t)resp_h << 8);
  } else {
    goto do_error;
  }

  status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_LEAT_SIGNIFICANT_BYTE, &resp_l, 1);
  if (status == I2C_SUCCESS) {
    *value |= (uint16_t)resp_l;
  } else {
    goto do_error;
  }

  goto do_exit;

do_error:
  *value = 0xFFFE;

do_exit:
  return status;
}

