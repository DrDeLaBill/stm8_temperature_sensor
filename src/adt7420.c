#include "adt7420.h"

//******************************************************************************
//Функция выдачи состояния выполнения операции adt7420 на основе состояния
//выполнения операции с I2C
//******************************************************************************
static t_adt7420_status adt7420_i2c_error(t_i2c_status status){
    if(status != I2C_SUCCESS){
      switch(status){
        case I2C_TIMEOUT:  return ADT7420_TIMEOUT; break;
        case I2C_ERROR:    return ADT7420_ERROR;   break;
        default:           return ADT7420_ERROR;   break;
      }
    } else {
        return ADT7420_SUCCESS;
    }
}

//******************************************************************************
//Проверка на наличие ошибок выполнения операции I2C
//******************************************************************************
#define adt7420_check_error(i2c_status)\
  if(i2c_status != I2C_SUCCESS)\
    return adt7420_i2c_error(i2c_status)


//******************************************************************************
//Инициализация adt7420
//******************************************************************************      
t_adt7420_status adt7420_init()
{
    //Состояние выполнения операции I2C
    t_i2c_status status = I2C_SUCCESS;
    
    //Переменная для хранения прочитанных данных
    uint8_t data;
  
    //Инициализация RTC. Читаем 0-й регистр
    status = i2c_rd_reg(ADT7420_SLAVE_ADDR, ADT7420_ID, (uint8_t*) &data, 1);

    if (data != 0xCB) {
      status = I2C_ERROR;
    }
    
    //Проверка состояния выполнении операции по I2C
    adt7420_check_error(status);
    
    //Если работа часов запрещена, то разрешаем сбросом CH=0
    // if(data.ch){
    //   //Сброс даты и времени
    //   return ds1307_reset(time_mode);
    // }
  
    return ADT7420_SUCCESS;
}

//******************************************************************************
//Чтение температуры из регистров ADT7420
//******************************************************************************
t_adt7420_status adt7420_get_temperature(int16_t* value)
{
  //Состояние выполнения операции I2C
  t_i2c_status status = I2C_SUCCESS;

  uint8_t data[2] = {0};

  //Чтение регистров adt7420
  status = i2c_rd_reg(ADT7420_SLAVE_ADDR, ADT7420_MOST_SIGNIFICANT_BYTE, (uint8_t*)&data, sizeof(data));

  //Проверка состояния выполнении операции по I2C
  adt7420_check_error(status);

  int16_t sign = (data[0] & 0b10000000) ? (-1) : 1;
  value = sign * (data[0] & 0b01111111);
  
  return ADT7420_SUCCESS;
}

