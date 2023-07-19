#ifndef _MODBUS_MANAGER_H_
#define _MODBUS_MANAGER_H_


#include <stdint.h>

#include "stm8s.h"
#include "mb-table.h"


#define MODBUS_BUF_SIZE 20


typedef struct modbus_data_status_typedef {
    uint8_t data[TABLE_Holding_Registers_Size + 10];
    uint8_t length;
    uint32_t start_time;
    bool wait_request_byte;
} modbus_data_status;


extern modbus_data_status modbus_data;


void modbus_manager_init();
void modbus_proccess();


#endif