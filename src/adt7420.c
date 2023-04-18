#include "adt7420.h"

#include "stm8s.h"
#include "i2c.h"
#include "main.h"
#include "utils.h"
#include "mb.h"
#include "mb-table.h"


uint32_t adt7420_delay = 0;


void adt7420_proccess()
{
  if ((uint32_t)ABS_DIF(Global_time, adt7420_delay) < ADT7420_DELAY) {
    return;
  }
  adt7420_delay = Global_time;
  int16_t tempr = 0;
  t_i2c_status status = adt7420_get_temperature(&tempr);
  if (status != I2C_SUCCESS) {
    tempr = 0;
  }
  mb_table_write(TABLE_Holding_Registers, 0, tempr);
}

t_i2c_status adt7420_init()
{
    i2c_master_init(get_clock_freq(), F_I2C_HZ);
    return adt7420_available();
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
  }

  status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_LEAT_SIGNIFICANT_BYTE, &resp_l, 1);
  if (status == I2C_SUCCESS) {
    *value |= (uint16_t)resp_l;
  }
  
  return status;
}

