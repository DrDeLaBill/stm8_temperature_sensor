#ifndef _MODBUS_MANAGER_H_
#define _MODBUS_MANAGER_H_


#include <stdint.h>

#include "stm8s.h"
#include "utils.h"
#include "modbus_rtu_slave.h"


#define MODBUS_BUF_SIZE 20


#define MODBUS_DATA_SIZE (MODBUS_SLAVE_OUTPUT_HOLDING_REGISTERS_COUNT > MODBUS_SLAVE_INPUT_REGISTERS_COUNT ? MODBUS_SLAVE_OUTPUT_HOLDING_REGISTERS_COUNT : MODBUS_SLAVE_INPUT_REGISTERS_COUNT + 10)
typedef struct modbus_data_status_typedef {
    uint8_t req[MODBUS_DATA_SIZE];
    uint8_t req_counter;
    uint8_t proc_counter;

    uint8_t resp[MODBUS_DATA_SIZE];
    uint8_t length;
    timer_t wait_timer;
    timer_t byte_timer;
    bool wait_request_byte;
    bool state_in_progress;
    bool is_response_ready;
} modbus_data_status;


extern modbus_data_status modbus_data;


void modbus_manager_init();
void modbus_proccess();
bool is_modbus_busy();
void modbus_proccess_byte(uint8_t byte);


#endif