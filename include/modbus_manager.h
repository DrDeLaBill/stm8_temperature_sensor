#ifndef _MODBUS_MANAGER_H_
#define _MODBUS_MANAGER_H_


#include <stdint.h>

#include "stm8s.h"
#include "utils.h"
#include "modbus_rtu_slave.h"


#define MODBUS_BUF_SIZE 20


typedef struct modbus_data_status_typedef {
    uint8_t data[MODBUS_REGISTER_SIZE + 10];
    uint8_t length;
    timer_t wait_timer;
    bool wait_request_byte;
    bool state_in_progress;
} modbus_data_status;


extern modbus_data_status modbus_data;


void modbus_manager_init();
void modbus_proccess();
bool is_modbus_busy();
void modbus_proccess_byte(uint8_t byte);


#endif