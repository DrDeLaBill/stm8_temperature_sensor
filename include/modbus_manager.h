#ifndef _MODBUS_MANAGER_H_
#define _MODBUS_MANAGER_H_


#include <stdint.h>


#define MODBUS_BUF_SIZE 20


typedef struct modbus_data_status_typedef {
    uint8_t data[MODBUS_BUF_SIZE];
    uint8_t length;
} modbus_data_status;


void modbus_manager_init();
void modbus_proccess();


#endif