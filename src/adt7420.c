#include "adt7420.h"

#include <stdbool.h>

#include "i2c.h"
#include "main.h"
#include "utils.h"
#include "modbus_rtu_slave.h"


#define MEASURE_DELAY_MS (uint32_t)1000 

#define CONFIG_SLEEP     (uint8_t)0b01100000
#define CONFIG_READ      (uint8_t)0b01000000
#define STATUS_RDY       (uint8_t)0b10000000


adt7420_state_t adt7420_state;


void _fsm_adt7420_state_enable_sens();
void _fsm_adt7420_state_wait_measure();
void _fsm_adt7420_state_save_measure();
void _fsm_adt7420_state_disable_sens();
void _fsm_adt7420_state_wait();

void _adt7420_set_action(void (*new_action) (void));
void _adt7420_clear_state();
void _adt7420_set_error_temp();
i2c_status_t _adt7420_get_temperature(int16_t* value);
i2c_status_t _adt7420_get_sensor_status(uint8_t* value);
i2c_status_t _adt7420_set_sensor_enable_status(bool enabled);


i2c_status_t adt7420_init()
{
  adt7420_state.adt_init_success = true;

  i2c_master_init(get_clock_freq(), F_I2C_HZ);
  
  i2c_status_t status = adt7420_available();
  if (status != I2C_SUCCESS) {
    goto do_error;
  }

  status = _adt7420_set_sensor_enable_status(false);
  if (status != I2C_SUCCESS) {
    goto do_error;
  }

  goto do_exit;

do_error:
  adt7420_state.adt_init_success = false;
  goto do_exit;

do_exit:
  return status;
}

void adt7420_proccess()
{
  if (!adt7420_state.adt_init_success) {
    _adt7420_set_error_temp();
    return;
  }

  if (!is_timer_wait(&adt7420_state.wait_timer)) {
    _adt7420_clear_state();
  }

  adt7420_state.state_action();
}

i2c_status_t adt7420_available() 
{
  uint8_t resp = 0;
  i2c_status_t status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_ID, &resp, 1);

  if ((resp & 0xF0) != 0xC0) {
    return I2C_ERROR;
  }

  return status;
}

bool adt7420_is_measurments_done()
{
  return adt7420_state.measurments_done;
}

i2c_status_t _adt7420_get_temperature(int16_t* value)
{
  //Чтение регистров adt7420
  uint8_t resp_h = 0;
  uint8_t resp_l = 0;

  i2c_status_t status = i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_MOST_SIGNIFICANT_BYTE, &resp_h, 1);
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
  *value = (int16_t)0xFFFF;

do_exit:
  return status;
}

i2c_status_t _adt7420_set_sensor_enable_status(bool enabled)
{
  uint8_t conf = enabled ? CONFIG_READ : CONFIG_SLEEP;
  return i2c_wr_reg(I2C_ADT7420_ADDR, ADT7420_CONFIGURATION, &conf, 1);
}

i2c_status_t _adt7420_get_sensor_status(uint8_t* value)
{
  return i2c_rd_reg(I2C_ADT7420_ADDR, ADT7420_STATUS, value, 1);
}

void _adt7420_clear_state() 
{
  adt7420_state.measurments_done = false;
  _adt7420_set_action(&_fsm_adt7420_state_enable_sens);
}

void _adt7420_set_error_temp()
{
  set_modbus_register_value(MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS, TEMPERATURE_REGISTER, 0xFFFF);
}

void _adt7420_set_action(void (*new_action) (void))
{
  if (new_action == _fsm_adt7420_state_enable_sens ||
      new_action == _fsm_adt7420_state_wait_measure ||
      new_action == _fsm_adt7420_state_save_measure ||
      new_action == _fsm_adt7420_state_disable_sens ||
      new_action == _fsm_adt7420_state_wait) 
  {
    adt7420_state.state_action = new_action;
  } else {
    adt7420_state.state_action = &_fsm_adt7420_state_enable_sens;
  }
  timer_start(&adt7420_state.wait_timer, MEASURE_DELAY_MS);
}

void _fsm_adt7420_state_enable_sens() 
{
  if (_adt7420_set_sensor_enable_status(true) == I2C_SUCCESS) {
    _adt7420_set_action(&_fsm_adt7420_state_wait_measure);
  } else {
    _adt7420_set_error_temp();
  }
}

void _fsm_adt7420_state_wait_measure()
{
  uint8_t status = 0x00;
  if (_adt7420_get_sensor_status(&status) != I2C_SUCCESS) {
    return;
  }
  if (!(status & STATUS_RDY)) {
    _adt7420_set_action(&_fsm_adt7420_state_save_measure);
  }
}

void _fsm_adt7420_state_save_measure()
{
  int16_t temperature = 0x0000;
  if (_adt7420_get_temperature(&temperature) == I2C_SUCCESS) {
    uint16_t buf = (((uint16_t)((temperature & 0xFF00) >> 8) & 0x7F) << 5) | ((uint8_t)(temperature & 0x00FF) >> 3);
    int16_t res = (int16_t)((buf * 10) / 16);
    bool sign = 0x80 & ((temperature & 0xFF00) >> 8);
    res *= (sign ? -1 : 1);
    set_modbus_register_value(MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS, TEMPERATURE_REGISTER, res);

    _adt7420_set_action(&_fsm_adt7420_state_disable_sens);
  }
}

void _fsm_adt7420_state_disable_sens()
{
  if (_adt7420_set_sensor_enable_status(true) == I2C_SUCCESS) {
    adt7420_state.measurments_done = true;
    _adt7420_set_action(&_fsm_adt7420_state_wait);
  }
}

void _fsm_adt7420_state_wait() {}