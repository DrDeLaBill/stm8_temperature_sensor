#ifndef _MODBUS_PROTOCOL_H_
#define _MODBUS_PROTOCOL_H_


#include <stdint.h>


typedef enum _MODE_T {
    MODE_SLAVE = 0x00,
    MODE_MASTER
} MODE_T;

typedef struct _modbus_state_t {
    MODE_T mode;
    void (*recieved_byte_handler) (uint8_t byte);
} modbus_state_t;


void set_recieved_byte_handler(void (*recieved_byte_handler) (void));


#endif