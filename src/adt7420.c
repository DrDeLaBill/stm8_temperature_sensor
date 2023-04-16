#include "adt7420.h"

#include "stm8s.h"
#include "i2c.h"
#include "stm8s_it.h"
#include "utils.h"
#include "modbus/mb.h"
#include "modbus/mb-table.h"


uint32_t adt7420_delay = 0;


void adt7420_proccess()
{
  // if (ABS_DIF(Global_time, adt7420_delay) < ADT7420_DELAY) {
  //   return;
  // }
  adt7420_delay = Global_time;
  int16_t tempr = 1;
  t_adt7420_status status = adt7420_get_temperature(&tempr);
  if (status != ADT7420_SUCCESS) {
    tempr = ADT7420_ERROR_RESP;
  }
  mb_table_write(TABLE_Holding_Registers, 0, tempr);
}

//******************************************************************************
//Функция выдачи состояния выполнения операции adt7420 на основе состояния
//выполнения операции с I2C
//******************************************************************************
t_adt7420_status adt7420_i2c_error(i2c_status status)
{
    if(status != I2C_OK){
      switch(status){
        case I2C_TMOUT:  return ADT7420_TIMEOUT; break;
        default:           return ADT7420_ERROR;   break;
      }
    } else {
        return ADT7420_SUCCESS;
    }
}

//******************************************************************************
//Проверка на наличие ошибок выполнения операции I2C
//******************************************************************************
t_adt7420_status adt7420_check_error(i2c_status status) 
{
  if(status != I2C_OK) {
    return adt7420_i2c_error(status);
  }
  return ADT7420_SUCCESS;
}


//******************************************************************************
//Инициализация adt7420
//******************************************************************************      
t_adt7420_status adt7420_init()
{
    i2c_setup();
    i2c_set_addr7(ADT7420_SLAVE_ADDR);
    return adt7420_available();
}

//******************************************************************************
//read device ID
//******************************************************************************
t_adt7420_status adt7420_available() 
{
  uint8_t resp = 0;
  i2c_status status = i2c_7bit_send_onebyte(ADT7420_ID, 1);

  if(status == I2C_OK){
    status = i2c_7bit_receive_onebyte(&resp, 1);
  }

  if ((resp & 0xF0) != 0xC0) {
    return ADT7420_ERROR;
  }

  return adt7420_check_error(status);
}

//******************************************************************************
//Чтение температуры из регистров ADT7420
//******************************************************************************
t_adt7420_status adt7420_get_temperature(int16_t* value)
{
  //Чтение регистров adt7420
  uint8_t resp = 0;
  i2c_status status = i2c_7bit_send_onebyte(ADT7420_MOST_SIGNIFICANT_BYTE, 1);

  if(status == I2C_OK) {
    status = i2c_7bit_receive_onebyte(&resp, 1);
  } else {
    // *value = status + 0x0100;
    // return adt7420_check_error(status);
  }

  if (status == I2C_OK) {
    *value = resp;
  } else {
    // *value = status + 0x0200;
    // return adt7420_check_error(status);
  }
  
  return adt7420_check_error(status);
}

