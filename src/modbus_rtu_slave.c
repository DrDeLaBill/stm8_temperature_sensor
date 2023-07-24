#include "modbus_rtu_slave.h"


#include <string.h>
#include <stdbool.h>
#include "modbus_manager.h"


#define SPECIAL_DATA_REGISTERS_COUNT_IDX 0
#define SPECIAL_DATA_VALUE_SIZE          2
#define SPECIAL_DATA_META_COUNT          3

#define MODBUS_ERROR_COMMAND_CODE        0x80

#define MODBUS_RESPONSE_MESSAGE_SIZE     (MODBUS_MESSAGE_DATA_SIZE + 5)


#define MODBUS_ENABLE_READ_COIL_STATUS          MODBUS_ENABLE_COMMAND_READ_COIL_STATUS          && MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
#define MODBUS_ENABLE_READ_INPUT_STATUS         MODBUS_ENABLE_COMMAND_READ_INPUT_STATUS         && MODBUS_ENABLE_DISCRETE_INPUT_COILS
#define MODBUS_ENABLE_READ_HOLDING_REGISTERS    MODBUS_ENABLE_COMMAND_READ_HOLDING_REGISTERS    && MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
#define MODBUS_ENABLE_READ_INPUT_REGISTERS      MODBUS_ENABLE_COMMAND_READ_INPUT_REGISTERS      && MODBUS_ENABLE_ANALOG_INPUT_REGISTERS
#define MODBUS_ENABLE_FORCE_SINGLE_COIL         MODBUS_ENABLE_COMMAND_FORCE_SINGLE_COIL         && MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
#define MODBUS_ENABLE_PRESET_SINGLE_REGISTER    MODBUS_ENABLE_COMMAND_PRESET_SINGLE_REGISTER    && MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
#define MODBUS_ENABLE_FORCE_MULTIPLE_COILS      MODBUS_ENABLE_COMMAND_FORCE_MULTIPLE_COILS      && MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
#define MODBUS_ENABLE_PRESET_MULTIPLE_REGISTERS MODBUS_ENABLE_COMMAND_PRESET_MULTIPLE_REGISTERS && MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS


#if MODBUS_ENABLE_DISCRETE_OUTPUT_COILS == true
uint16_t mb_discrete_output_coils[MODBUS_REGISTER_SIZE] = {0};
#endif
#if MODBUS_ENABLE_DISCRETE_INPUT_COILS == true
uint16_t mb_discrete_input_coils[MODBUS_REGISTER_SIZE] = {0};
#endif
#if MODBUS_ENABLE_ANALOG_INPUT_REGISTERS == true
uint16_t mb_analog_input_registers[MODBUS_REGISTER_SIZE] = {0};
#endif
#if MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS == true
uint16_t mb_analog_output_holding_registers[MODBUS_REGISTER_SIZE] = {0};
#endif


modbus_state_t mb_state = {
    .slave_id = 0x00,
    .response_data_handler = NULL,
    .request_byte_handler = _mb_fsm_request_slave_id,
    .data_req = {
        .id = 0x00,
        .command = 0x00,
        .register_addr = 0x0000,
        .crc = 0x0000
    },
    .data_resp = {
        .id = 0x00,
        .command = 0x00,
        .data_len = 0x00,
        .crc = 0x0000
    },
    .data_handler_counter = 0,
    .is_error_response = false
};


void _mb_set_request_byte_handler(void (*request_byte_handler) (uint8_t));
void _mb_reset_data();

void _mb_fsm_request_slave_id(uint8_t byte);
void _mb_fsm_request_command(uint8_t byte);
void _mb_fsm_request_register_addr(uint8_t byte);
void _mb_fsm_request_special_data(uint8_t byte);
void _mb_fsm_request_crc(uint8_t byte);

void _mb_request_proccess();
void _mb_write_single_register();
void _mb_write_multiple_registers();
void _mb_make_error_response(modbus_error_types_t error_type);
void _mb_send_response();

bool _mb_is_read_command();
bool _mb_is_write_single_reg_command();
bool _mb_is_write_multiple_reg_command();
bool _mb_is_recieved_own_slave_id();
bool _mb_check_request_command();
bool _mb_check_request_register_addr();
bool _mb_check_request_registers_count();
uint16_t _mb_get_special_data_first_value();
uint16_t _mb_get_needed_data_len();
uint16_t _mb_get_needed_registers_count();

void _mb_make_read_response();
void _mb_make_write_single_response();
void _mb_make_write_multiple_response();

uint16_t _mb_crc16(const uint8_t *data, uint16_t len);


void modbus_set_response_data_handler(void (*response_data_handler) (uint8_t*, uint8_t)) 
{
    if (response_data_handler != NULL) {
        mb_state.response_data_handler = response_data_handler;
    }
}

void modbus_recieve_data_byte(uint8_t byte) 
{
    if (mb_state.request_byte_handler != NULL) {
        mb_state.data_handler_counter++;
        mb_state.request_byte_handler(byte);
    } else {
        _mb_reset_data();
    }
}

void modbus_set_slave_id(uint8_t new_slave_id) 
{
    mb_state.slave_id = new_slave_id;
    _mb_reset_data();
}

void modbus_timeout()
{
    _mb_reset_data();
}

uint16_t get_modbus_register_value(register_type_t register_type, uint16_t register_id)
{
    if (register_id >= MODBUS_REGISTER_SIZE) {
        return 0;
    }
#if MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
    if (register_type == MODBUS_REGISTER_DISCRETE_OUTPUT_COILS) {
        return mb_discrete_output_coils[register_id];
    }
#endif
#if MODBUS_ENABLE_DISCRETE_INPUT_COILS
    if (register_type == MODBUS_REGISTER_DISCRETE_INPUT_COILS) {
        return mb_discrete_input_coils[register_id];
    }
#endif
#if MODBUS_ENABLE_ANALOG_INPUT_REGISTERS
    if (register_type == MODBUS_REGISTER_ANALOG_INPUT_REGISTERS) {
        return mb_analog_input_registers[register_id];
    }
#endif
#if MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
    if (register_type == MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS) {
        return mb_analog_output_holding_registers[register_id];
    }
#endif
    return 0;
}

void set_modbus_register_value(register_type_t register_type, uint16_t register_id, uint16_t value)
{
    if (register_id >= MODBUS_REGISTER_SIZE) {
        return;
    }
#if MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
    if (register_type == MODBUS_REGISTER_DISCRETE_OUTPUT_COILS) {
        mb_discrete_output_coils[register_id] = value;
    }
#endif
#if MODBUS_ENABLE_DISCRETE_INPUT_COILS
    if (register_type == MODBUS_REGISTER_DISCRETE_INPUT_COILS) {
        mb_discrete_input_coils[register_id] = value;
    }
#endif
#if MODBUS_ENABLE_ANALOG_INPUT_REGISTERS
    if (register_type == MODBUS_REGISTER_ANALOG_INPUT_REGISTERS) {
        mb_analog_input_registers[register_id] = value;
    }
#endif
#if MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
    if (register_type == MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS) {
        mb_analog_output_holding_registers[register_id] = value;
    }
#endif
}

void _mb_request_proccess()
{
    if (!_mb_is_recieved_own_slave_id()) {
        _mb_reset_data();
        return;
    }
    if (mb_state.response_data_handler == NULL) {
        return;
    }

    mb_state.data_resp.id = mb_state.data_req.id;

    /* CHECK ERRORS BEGIN */
    if (!_mb_check_request_command()) {
        _mb_make_error_response(MODBUS_ERROR_ILLEGAL_FUNCTION);
        goto do_send;
    }

    if (!_mb_check_request_register_addr()) {
        _mb_make_error_response(MODBUS_ERROR_ILLEGAL_DATA_ADDRESS);
        goto do_send;
    }

    if (!_mb_check_request_registers_count()) {
        _mb_make_error_response(MODBUS_ERROR_ILLEGAL_DATA_VALUE);
        goto do_send;
    }
    /* CHECK ERRORS END */

    mb_state.data_resp.command = mb_state.data_req.command;

    /* WRITE REGISTERS BEGIN */
    if (_mb_is_write_single_reg_command()) {
        _mb_write_single_register();
    }

    if (_mb_is_write_multiple_reg_command()) {
        _mb_write_multiple_registers();
    }
    /* WRITE REGISTERS END */

    /* MAKE RESPONSE DATA BEGIN */
    if (_mb_is_read_command()) {
        _mb_make_read_response();
    }
    if (_mb_is_write_single_reg_command()) {
        _mb_make_write_single_response();
    }
    if (_mb_is_write_multiple_reg_command()) {
        _mb_make_write_multiple_response();
    }
    /* MAKE RESPONSE DATA END */

    goto do_send;

do_send:
    _mb_send_response();
    _mb_reset_data();
}

void _mb_make_error_response(modbus_error_types_t error_type)
{
    mb_state.is_error_response = true;
    mb_state.data_resp.command = mb_state.data_req.command | MODBUS_ERROR_COMMAND_CODE;
    mb_state.data_resp.data_resp[0] = error_type;
}

void _mb_send_response()
{
    if (mb_state.response_data_handler == NULL) {
        return;
    }

    uint8_t data[MODBUS_RESPONSE_MESSAGE_SIZE] = {0};
    uint16_t counter = 0;
    data[counter++] = mb_state.data_resp.id;
    data[counter++] = mb_state.data_resp.command;
    if (mb_state.is_error_response) {
        data[counter++] = mb_state.data_resp.data_resp[0];
    } else {
        memcpy(data + counter, mb_state.data_resp.data_resp, mb_state.data_resp.data_len);
        counter += mb_state.data_resp.data_len;
    }
    uint16_t crc = _mb_crc16(data, counter);
    data[counter++] = crc & 0xFF;
    data[counter++] = crc >> 8;

    mb_state.response_data_handler(data, counter);
}

void _mb_write_single_register()
{
#if MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
    if (mb_state.data_req.command == MODBUS_FORCE_SINGLE_COIL) {
        mb_discrete_output_coils[mb_state.data_req.register_addr] = _mb_get_special_data_first_value();
    }
#endif
#if MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
    if (mb_state.data_req.command == MODBUS_PRESET_SINGLE_REGISTER) {
        mb_analog_output_holding_registers[mb_state.data_req.register_addr] = _mb_get_special_data_first_value();
    }
#endif
}

void _mb_write_multiple_registers()
{
    uint8_t count = mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT - 1];
    if (mb_state.data_req.register_addr >= MODBUS_REGISTER_SIZE) {
        return;
    }
    if (SPECIAL_DATA_META_COUNT + count - 1 > MODBUS_REGISTER_SIZE) {
        count = MODBUS_REGISTER_SIZE - SPECIAL_DATA_META_COUNT;
    }
#if MODBUS_ENABLE_DISCRETE_OUTPUT_COILS
    if (mb_state.data_req.command == MODBUS_FORCE_MULTIPLE_COILS) {
        for (uint8_t i = 0; i < count; i++) {
            uint16_t value = (uint16_t)mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT + i];
            mb_discrete_output_coils[mb_state.data_req.register_addr + i] = value;
        }
    }
#endif
#if MODBUS_ENABLE_ANALOG_OUTPUT_HOLDING_REGISTERS
    if (mb_state.data_req.command == MODBUS_PRESET_MULTIPLE_REGISTERS) {
        for (uint8_t i = 0; i < count; i += 2) {
            uint16_t value = (uint16_t)mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT + i] << 8 |
                             (uint16_t)mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT + i + 1];
            mb_analog_output_holding_registers[mb_state.data_req.register_addr + i / 2] = value;
        }
    }
#endif
}

void _mb_reset_data() 
{
    memset((uint8_t*)&mb_state.data_req, 0, sizeof(mb_state.data_req));
    memset((uint8_t*)&mb_state.data_resp, 0, sizeof(mb_state.data_resp));

    mb_state.is_error_response = false;
    mb_state.request_byte_handler = _mb_fsm_request_slave_id;
    mb_state.data_handler_counter = 0;
    // _mb_set_request_byte_handler(_mb_fsm_request_slave_id);
}

void _mb_set_request_byte_handler(void (*request_byte_handler) (uint8_t))
{
    if (
        !memcmp(*request_byte_handler, _mb_fsm_request_slave_id, sizeof(*request_byte_handler)) ||
        !memcmp(*request_byte_handler, _mb_fsm_request_command, sizeof(*request_byte_handler)) ||
        !memcmp(*request_byte_handler, _mb_fsm_request_register_addr, sizeof(*request_byte_handler)) ||
        !memcmp(*request_byte_handler, _mb_fsm_request_special_data, sizeof(*request_byte_handler)) ||
        !memcmp(*request_byte_handler, _mb_fsm_request_crc, sizeof(*request_byte_handler))
    )
    {
        mb_state.data_handler_counter = 0;
        mb_state.request_byte_handler = request_byte_handler;
    }
}

void _mb_make_read_response()
{
    uint8_t command = mb_state.data_req.command;
    uint8_t req_data_len = _mb_get_special_data_first_value();

    uint8_t counter = 0;
    while (counter < req_data_len) {
        uint16_t cur_idx = mb_state.data_req.register_addr + counter;
#if MODBUS_ENABLE_READ_COIL_STATUS
        if (command == MODBUS_READ_COILS) {
            mb_state.data_resp.data_resp[1 + counter] = mb_discrete_output_coils[cur_idx];
        }
#endif
#if MODBUS_ENABLE_READ_INPUT_STATUS
        if (command == MODBUS_READ_INPUT_STATUS) {
            mb_state.data_resp.data_resp[1 + counter] = mb_discrete_input_coils[cur_idx];
        }
#endif
#if MODBUS_ENABLE_READ_HOLDING_REGISTERS
        if (command == MODBUS_READ_HOLDING_REGISTERS) {
            mb_state.data_resp.data_resp[1 + counter * 2] = mb_analog_output_holding_registers[cur_idx] >> 8;
            mb_state.data_resp.data_resp[1 + counter * 2 + 1] = mb_analog_output_holding_registers[cur_idx];
        }
#endif
#if MODBUS_ENABLE_READ_INPUT_REGISTERS
        if (command == MODBUS_READ_INPUT_REGISTERS) {
            mb_state.data_resp.data_resp[1 + counter * 2] = mb_analog_input_registers[cur_idx] >> 8;
            mb_state.data_resp.data_resp[1 + counter * 2 + 1] = mb_analog_input_registers[cur_idx];
        }
#endif
        counter++;
    }
    
    uint8_t resp_data_len = 0;
    if (command == MODBUS_READ_HOLDING_REGISTERS || command == MODBUS_READ_INPUT_REGISTERS) {
        resp_data_len = counter * 2;
    } else {
        resp_data_len = counter;
    }
    mb_state.data_resp.data_len = 1 + resp_data_len;
    mb_state.data_resp.data_resp[0] = resp_data_len;
}

void _mb_make_write_single_response()
{
    uint8_t counter = 0;

    uint8_t command = mb_state.data_req.command;
    uint16_t reg_addr = mb_state.data_req.register_addr;
    
    mb_state.data_resp.data_resp[counter++] = reg_addr >> 8;
    mb_state.data_resp.data_resp[counter++] = reg_addr;

#if MODBUS_ENABLE_FORCE_SINGLE_COIL
    if (mb_state.data_req.command == MODBUS_FORCE_SINGLE_COIL) { 
        mb_state.data_resp.data_resp[counter++] = mb_discrete_output_coils[reg_addr] >> 8;
        mb_state.data_resp.data_resp[counter++] = mb_discrete_output_coils[reg_addr];
    }
#endif
#if MODBUS_ENABLE_PRESET_SINGLE_REGISTER
    if (mb_state.data_req.command == MODBUS_PRESET_SINGLE_REGISTER) { 
        mb_state.data_resp.data_resp[counter++] = mb_analog_output_holding_registers[reg_addr] >> 8;
        mb_state.data_resp.data_resp[counter++] = mb_analog_output_holding_registers[reg_addr];
    }
#endif

    mb_state.data_resp.data_len = counter;
}

void _mb_make_write_multiple_response()
{
    uint8_t counter = 0;

    uint16_t written_count = mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT - 1];

    uint16_t reg_addr = mb_state.data_req.register_addr;
    
    mb_state.data_resp.data_resp[counter++] = reg_addr >> 8;
    mb_state.data_resp.data_resp[counter++] = reg_addr;
    mb_state.data_resp.data_resp[counter++] = written_count >> 8;
    mb_state.data_resp.data_resp[counter++] = written_count;

    mb_state.data_resp.data_len = counter;
}

void _mb_fsm_request_slave_id(uint8_t byte)
{
    _mb_reset_data();
    mb_state.data_req.id = byte;
    mb_state.data_handler_counter = 0;
    mb_state.request_byte_handler = _mb_fsm_request_command;
    // _mb_set_request_byte_handler(_mb_fsm_request_command);
}

void _mb_fsm_request_command(uint8_t byte)
{
    mb_state.data_req.command = byte;
    mb_state.data_handler_counter = 0;
    mb_state.request_byte_handler = _mb_fsm_request_register_addr;
    uint8_t command = mb_state.data_req.command;
}

void _mb_fsm_request_register_addr(uint8_t byte)
{
    mb_state.data_req.register_addr <<= 8;
    mb_state.data_req.register_addr |= byte;
    if (mb_state.data_handler_counter == sizeof(mb_state.data_req.register_addr)) {
        mb_state.data_handler_counter = 0;
        mb_state.request_byte_handler = _mb_fsm_request_special_data;
        // _mb_set_request_byte_handler(_mb_fsm_request_special_data);
    }
    if (mb_state.data_req.register_addr >= MODBUS_REGISTER_SIZE) {
        _mb_reset_data();
    }
}

void _mb_fsm_request_special_data(uint8_t byte)
{
    if (mb_state.data_req.register_addr + _mb_get_needed_registers_count() >= MODBUS_REGISTER_SIZE) {
        _mb_reset_data();
        return;
    }

    if (mb_state.data_handler_counter > sizeof(mb_state.data_req.special_data)) {
        _mb_reset_data();
        return;
    }

    mb_state.data_req.special_data[mb_state.data_handler_counter - 1] = byte;

    uint16_t needed_count = SPECIAL_DATA_VALUE_SIZE;

    if (_mb_is_write_multiple_reg_command()) {
        needed_count = SPECIAL_DATA_META_COUNT + mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT-1];
    } 

    if (mb_state.data_handler_counter == needed_count) {
        mb_state.data_handler_counter = 0;
        mb_state.request_byte_handler = _mb_fsm_request_crc;
        // _mb_set_request_byte_handler(_mb_fsm_request_crc);
    }
    
}

void _mb_fsm_request_crc(uint8_t byte)
{
    mb_state.data_req.crc <<= 8;
    mb_state.data_req.crc |= byte;

    if (mb_state.data_handler_counter < sizeof(mb_state.data_req.crc)) {
        return;
    }

    if (!_mb_is_recieved_own_slave_id()) {
        mb_state.data_handler_counter = 0;
        mb_state.request_byte_handler = _mb_fsm_request_slave_id;
        // _mb_set_request_byte_handler(_mb_fsm_request_slave_id);
        return;
    }

    _mb_request_proccess();
    _mb_reset_data();
}

bool _mb_is_read_command()
{
    return !_mb_is_write_multiple_reg_command() && !_mb_is_write_single_reg_command();
}

bool _mb_is_write_single_reg_command()
{
    return mb_state.data_req.command == MODBUS_FORCE_SINGLE_COIL || mb_state.data_req.command == MODBUS_PRESET_SINGLE_REGISTER;
}

bool _mb_is_write_multiple_reg_command()
{
    return mb_state.data_req.command == MODBUS_FORCE_MULTIPLE_COILS || mb_state.data_req.command == MODBUS_PRESET_MULTIPLE_REGISTERS;
}

bool _mb_is_recieved_own_slave_id()
{
    return mb_state.slave_id == mb_state.data_req.id;
}

bool _mb_check_request_command() 
{
    return false
#if MODBUS_ENABLE_READ_COIL_STATUS
        || mb_state.data_req.command == MODBUS_READ_COILS
#endif
#if MODBUS_ENABLE_READ_INPUT_STATUS
        || mb_state.data_req.command == MODBUS_READ_INPUT_STATUS
#endif
#if MODBUS_ENABLE_READ_HOLDING_REGISTERS
        || mb_state.data_req.command == MODBUS_READ_HOLDING_REGISTERS
#endif
#if MODBUS_ENABLE_READ_INPUT_REGISTERS
        || mb_state.data_req.command == MODBUS_READ_INPUT_REGISTERS
#endif
#if MODBUS_ENABLE_FORCE_SINGLE_COIL
        || mb_state.data_req.command == MODBUS_FORCE_SINGLE_COIL
#endif
#if MODBUS_ENABLE_PRESET_SINGLE_REGISTER
        || mb_state.data_req.command == MODBUS_PRESET_SINGLE_REGISTER
#endif
#if MODBUS_ENABLE_FORCE_MULTIPLE_COILS
        || mb_state.data_req.command == MODBUS_FORCE_MULTIPLE_COILS
#endif
#if MODBUS_ENABLE_PRESET_MULTIPLE_REGISTERS
        || mb_state.data_req.command == MODBUS_PRESET_MULTIPLE_REGISTERS
#endif
    ;
}

bool _mb_check_request_register_addr()
{
    return mb_state.data_req.register_addr < MODBUS_REGISTER_SIZE;
}

bool _mb_check_request_registers_count()
{
    uint16_t reg_count = _mb_get_needed_registers_count();
    uint8_t data_count = mb_state.data_req.special_data[SPECIAL_DATA_META_COUNT - 1];
    uint16_t reg_addr  = mb_state.data_req.register_addr;
    return reg_count > 0 
        && reg_addr + reg_count < MODBUS_REGISTER_SIZE 
        && SPECIAL_DATA_META_COUNT + data_count <= sizeof(mb_state.data_req.special_data);
}

uint16_t _mb_get_special_data_first_value()
{
    uint8_t regh = mb_state.data_req.special_data[SPECIAL_DATA_REGISTERS_COUNT_IDX];
    uint8_t regl = mb_state.data_req.special_data[SPECIAL_DATA_REGISTERS_COUNT_IDX + 1];
    return (uint16_t)(((uint16_t)regh) << 8) + (uint16_t)regl;
}

uint16_t _mb_get_needed_registers_count()
{
    modbus_command_t command = mb_state.data_req.command;
#if MODBUS_ENABLE_FORCE_SINGLE_COIL || MODBUS_ENABLE_PRESET_SINGLE_REGISTER
    if (_mb_is_write_single_reg_command()) {
        return 1;
    }
#endif
#if MODBUS_ENABLE_READ_COIL_STATUS || MODBUS_ENABLE_READ_INPUT_STATUS || MODBUS_ENABLE_READ_HOLDING_REGISTERS || MODBUS_ENABLE_READ_INPUT_REGISTERS || MODBUS_ENABLE_FORCE_MULTIPLE_COILS || MODBUS_ENABLE_PRESET_MULTIPLE_REGISTERS
    if (!_mb_is_write_single_reg_command()) {
        return _mb_get_special_data_first_value();
    }
#endif
    return 0;
}

const uint16_t crc_table[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

uint16_t _mb_crc16(const uint8_t *data, uint16_t len)
{
    uint8_t tmp;
    uint16_t crc_word = 0xFFFF;
    while (len--)
    {
        tmp = *data++ ^ crc_word;
        crc_word >>= 8;
        crc_word ^= crc_table[tmp];
    }
    return crc_word;
}