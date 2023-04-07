#include "adt7420.h"

#include "stm8s.h"
#include "i2c.h"


//******************************************************************************
//Функция выдачи состояния выполнения операции adt7420 на основе состояния
//выполнения операции с I2C
//******************************************************************************
t_adt7420_status adt7420_i2c_error(i2c_status status){
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
    return si7005_available();
}

//******************************************************************************
//read device ID
//******************************************************************************
t_adt7420_status si7005_available() {
  uint8_t resp = 0;
  i2c_status status = i2c_7bit_send_onebyte(ADT7420_ID, 0);

  if(status == I2C_OK){
    status = i2c_7bit_receive_onebyte(&resp,0);
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
  uint8_t resp;
  i2c_status status = i2c_7bit_send_onebyte(ADT7420_MOST_SIGNIFICANT_BYTE, 0);

  if(status == I2C_OK){
    status = i2c_7bit_receive_onebyte(&resp,0);
  }

  int8_t sign = +1;
  if(status == I2C_OK){
    sign = (resp & 0b10000000) ? (-1) : 1;
    *value = sign * (resp & 0b01111111);
  }
  
  return adt7420_check_error(status);
}

