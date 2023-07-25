#ifndef _MODBUS_PROTOCOL_H_
#define _MODBUS_PROTOCOL_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


/* MODBUS SLAVE SETTINGS BEGIN */
// Registers:
#define MODBUS_ENABLE_DISCRETE_OUTPUT_COILS             false
#define MODBUS_ENABLE_DISCRETE_INPUT_COILS              false
#define MODBUS_ENABLE_ANALOG_INPUT_REGISTERS            true
#define MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS   true
#define MODBUS_REGISTER_SIZE                            16    // MODBUS default: 9999

// Commands:
#define MODBUS_ENABLE_COMMAND_READ_COIL_STATUS          false
#define MODBUS_ENABLE_COMMAND_READ_INPUT_STATUS         false
#define MODBUS_ENABLE_COMMAND_READ_HOLDING_REGISTERS    true
#define MODBUS_ENABLE_COMMAND_READ_INPUT_REGISTERS      true
#define MODBUS_ENABLE_COMMAND_FORCE_SINGLE_COIL         false
#define MODBUS_ENABLE_COMMAND_PRESET_SINGLE_REGISTER    true
#define MODBUS_ENABLE_COMMAND_FORCE_MULTIPLE_COILS      false
#define MODBUS_ENABLE_COMMAND_PRESET_MULTIPLE_REGISTERS false
/* MODBUS SLAVE SETTINGS END */


typedef enum _modbus_command_t {
    MODBUS_READ_COILS                = 0x01,
    MODBUS_READ_INPUT_STATUS         = 0x02,
    MODBUS_READ_HOLDING_REGISTERS    = 0x03,
    MODBUS_READ_INPUT_REGISTERS      = 0x04,
    MODBUS_FORCE_SINGLE_COIL         = 0x05,
    MODBUS_PRESET_SINGLE_REGISTER    = 0x06,
    MODBUS_FORCE_MULTIPLE_COILS      = 0x0F,
    MODBUS_PRESET_MULTIPLE_REGISTERS = 0x10
} modbus_command_t;


typedef enum _register_type_t {
    MODBUS_REGISTER_DISCRETE_OUTPUT_COILS           = 0x01,
    MODBUS_REGISTER_DISCRETE_INPUT_COILS            = 0x02,
    MODBUS_REGISTER_ANALOG_INPUT_REGISTERS          = 0x03,
    MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS = 0x04
} register_type_t;


typedef enum _modbus_error_types_t {
    MODBUS_ERROR_ILLEGAL_FUNCTION         = 0x01,
    MODBUS_ERROR_ILLEGAL_DATA_ADDRESS     = 0x02,
    MODBUS_ERROR_ILLEGAL_DATA_VALUE       = 0x03,
    MODBUS_ERROR_SLAVE_DEVICE_FAILURE     = 0x04,
    MODBUS_ERROR_ACKNOWLEDGE              = 0x05,
    MODBUS_ERROR_SLAVE_DEVICE_BUSY        = 0x06,
    MODBUS_ERROR_MEMORY_PARITY            = 0x08,
    MODBUS_ERROR_GATEWAY_PATH_UNAVAILABLE = 0x0A,
    MODBUS_ERROR_FAILED_TO_RESPOND        = 0x0B
} modbus_error_types_t;


#define MODBUS_MESSAGE_DATA_SIZE (2 * (MODBUS_REGISTER_SIZE + 1) + 1)
typedef struct _modbus_request_message_t {
    uint8_t id;
    uint8_t command;
    uint16_t register_addr;
    uint8_t special_data[MODBUS_MESSAGE_DATA_SIZE];
    uint16_t crc;
} modbus_request_message_t;


typedef struct _modbus_response_message_t {
    uint8_t id;
    uint8_t command;
    uint8_t data_len;
    uint8_t data_resp[MODBUS_MESSAGE_DATA_SIZE];
    uint16_t crc;
} modbus_response_message_t;


typedef struct _modbus_state_t {
    uint8_t slave_id;
    void (*response_data_handler) (uint8_t*, uint8_t);
    void (*request_byte_handler) (uint8_t);
    modbus_request_message_t data_req;
    uint8_t data_handler_counter;
    modbus_response_message_t data_resp;
    bool is_error_response;
} modbus_state_t;


void modbus_set_response_data_handler(void (*response_data_handler) (uint8_t*, uint8_t));
void modbus_recieve_data_byte(uint8_t byte);
void modbus_set_slave_id(uint8_t new_slave_id);
void modbus_timeout();

uint16_t get_modbus_register_value(register_type_t register_type, uint16_t register_id);
void set_modbus_register_value(register_type_t register_type, uint16_t register_id, uint16_t value);


#ifdef __cplusplus
}
#endif


#endif