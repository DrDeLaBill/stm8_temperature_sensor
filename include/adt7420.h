#ifndef _ADT7420_H_
#define _ADT7420_H_

#include <stdint.h>

#include "stm8s.h"
#include "i2c.h"
#include "main.h"
#include "utils.h"


//******************************************************************************
//Параметры ADT7420
//******************************************************************************

//I2C-адрес микросхемы
#define ADT7420_SLAVE_ADDR I2C_ADT7420_ADDR
// Ошибка чтения
#define ADT7420_ERROR_RESP -1

//******************************************************************************
//Адреса регистров ADT7420
//******************************************************************************
typedef enum {
  ADT7420_MOST_SIGNIFICANT_BYTE = 0x00,
  ADT7420_LEAT_SIGNIFICANT_BYTE,
  ADT7420_STATUS,
  ADT7420_CONFIGURATION,
  ADT7420_T_HIGH_M,
  ADT7420_T_HIGH_L,
  ADT7420_T_LOW_M,
  ADT7420_T_LOW_L,
  ADT7420_T_CRIT_M,
  ADT7420_T_CRIT_L,
  ADT7420_T_HYST,
  ADT7420_ID,
} t_adt7420_reg;

typedef struct _adt7420_state_t {
  void (*state_action) (void);
  bool adt_init_success;
  bool measurments_done;
  timer_t wait_timer;
} adt7420_state_t;

//******************************************************************************
// Прототипы функций
//******************************************************************************
void adt7420_proccess();
bool adt7420_is_measurments_done();
void adt7420_enable_sensor();
i2c_status_t adt7420_init();
i2c_status_t adt7420_available();

//******************************************************************************
// Макросы
//******************************************************************************


#endif