#include "modbus_manager.h"

#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "modbus_rtu_slave.h"
#include "uart.h"
#include "utils.h"
#include "settings.h"


#define MODBUS_DEFAULT_DELAY          5
#define MODBUS_BYTE_RECIEVE_TIME      2
#define MODBUS_CHECK(condition, time) { if (!wait_event(condition, time)) { return; } }


void _modbus_data_handler(uint8_t * data, uint32_t len);
void _internal_error_handler();
void _send_response();
void _update_mb_id_proccess();
void _clear_data();
bool _is_MAX485_ready();
bool _is_modbus_txe();
bool _is_modbus_rxne_empty();
bool _is_MAX485_free();


modbus_data_status modbus_data = {
    .length            = 0, 
    .req               = { 0 },
    .req_counter       = 0,

    .resp              = { 0 },
    .wait_timer        = { 0 },
    .byte_timer        = { 0 },
    .is_response_ready = false,
    .state_in_progress = false,
    .wait_request_byte = false,
};


void modbus_manager_init()
{
    uart_init(UART_BAUD_RATE, get_clock_freq());
    modbus_slave_set_slave_id(sttngs.mb_id);
    modbus_slave_set_response_data_handler(&_modbus_data_handler);
    modbus_slave_set_internal_error_handler(&_internal_error_handler);
    _clear_data();

    modbus_slave_set_register_value(MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS, SLAVE_ID_REGISTER, sttngs.mb_id);
    modbus_slave_set_register_value(MODBUS_REGISTER_ANALOG_INPUT_REGISTERS, VERSION_REGISTER, SENSOR_VERSION);
}

void modbus_proccess()
{
    _update_mb_id_proccess();

    if (modbus_data.state_in_progress && modbus_data.proc_counter < modbus_data.req_counter) {
        modbus_slave_receive_data_byte(modbus_data.req[modbus_data.proc_counter++]);
    }

    if (modbus_data.wait_request_byte && !is_timer_wait(&modbus_data.wait_timer)) {
        modbus_slave_timeout();
        _clear_data();
        return;
    }

    if (is_timer_wait(&modbus_data.byte_timer)) {
        return;
    }

    if (!modbus_data.length) {
        return;
    }
    
    _send_response();
    _clear_data();
}

void modbus_proccess_byte(uint8_t byte)
{
    timer_start(&modbus_data.wait_timer, MODBUS_TIMEOUT_MS);
    timer_start(&modbus_data.byte_timer, MODBUS_BYTE_RECIEVE_TIME);
    modbus_data.wait_request_byte = true;
    modbus_data.state_in_progress = true;
    if (!modbus_data.is_response_ready) {
        modbus_data.req[modbus_data.req_counter++] = byte;
    }
    if (modbus_data.req_counter >= sizeof(modbus_data.req)) {
        modbus_data.req_counter = 0;
        modbus_data.proc_counter = 0;
        modbus_data.req[modbus_data.req_counter++] = byte;
    }
}

bool is_modbus_busy()
{
    return modbus_data.state_in_progress;
}

void _update_mb_id_proccess() 
{
    uint16_t slave_id = modbus_slave_get_register_value(MODBUS_REGISTER_ANALOG_OUTPUT_HOLDING_REGISTERS, SLAVE_ID_REGISTER);
    if (sttngs.mb_id != slave_id) {
        sttngs_update_mb_id(slave_id);
        modbus_slave_set_slave_id(sttngs.mb_id);
    }
}

void _modbus_data_handler(uint8_t* data, uint32_t len)
{
    modbus_data.is_response_ready = true;

    if (!len || len > sizeof(modbus_data.resp)) {
        _clear_data();
        return;
    }
    
    modbus_data.length = len;
    memcpy(modbus_data.resp, data, len);
}

void _send_response()
{
    if (modbus_data.length > sizeof(modbus_data.resp)) {
        modbus_slave_clear_data();
        _clear_data();
        return;
    }
    
    delay_ms(1);
    MODBUS_CHECK(&_is_modbus_rxne_empty, MODBUS_DEFAULT_DELAY);

    GPIOD->ODR |= (uint8_t)(MAX485_PIN);
    MODBUS_CHECK(&_is_MAX485_free, MODBUS_DEFAULT_DELAY);

    uart_tx_data(modbus_data.resp, modbus_data.length);
    MODBUS_CHECK(&_is_modbus_txe, MODBUS_DEFAULT_DELAY);
    delay_ms(MODBUS_DEFAULT_DELAY);

    GPIOD->ODR &= ~(uint8_t)(MAX485_PIN);
    MODBUS_CHECK(&_is_MAX485_free, MODBUS_DEFAULT_DELAY);

    modbus_data.is_response_ready = false;
}

void _internal_error_handler()
{
    _clear_data();
}

void _clear_data()
{
    memset((uint8_t*)&modbus_data, 0, sizeof(modbus_data));
    timer_start(&modbus_data.wait_timer, MODBUS_TIMEOUT_MS);
    modbus_slave_clear_data();
}

bool _is_MAX485_ready()
{
    return !(GPIOD->IDR & RX_PIN);
}

bool _is_modbus_txe()
{
    return UART1->SR & UART1_SR_TXE;
}

bool _is_modbus_rxne_empty()
{
    return !(UART1->SR & UART1_SR_RXNE);
}

bool _is_MAX485_free()
{
    return !_is_MAX485_ready();
}