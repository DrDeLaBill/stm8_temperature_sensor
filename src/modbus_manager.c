#include "modbus_manager.h"

#include "main.h"
#include "mb.h"
#include "mb-table.h"
#include "uart.h"
#include "utils.h"


#define MODBUS_DEFAULT_DELAY          5
#define MODBUS_CHECK(condition, time) if (!wait_event(condition, time)) {return;}


void _modbus_data_handler(uint8_t * data, uint8_t len);
void _send_response();
void _clear_data();
bool _is_MAX485_ready();
bool _is_modbus_txe();
bool _is_MAX485_free();


modbus_data_status modbus_data;

void modbus_manager_init()
{
    uart_init(UART_BAUD_RATE, get_clock_freq());
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&_modbus_data_handler);
    _clear_data();
}

void modbus_proccess()
{
    if (!modbus_data.length) {
        return;
    }
    if (ABS_DIF(modbus_data.start_time, Global_time) > MODBUS_WAIT_TIME) {
        mb_rx_timeout_handler();
        _clear_data();
        return;
    }
    _send_response();
    _clear_data();
}


void _modbus_data_handler(uint8_t * data, uint8_t len)
{
    if (!len || len > sizeof(modbus_data.data)) {
        return;
    }
    _clear_data();
    modbus_data.length = len;
    for (uint8_t i = 0; i < len; i++) {
        modbus_data.data[i] = data[i];
    }
}

void _send_response()
{
    GPIOD->ODR |= (uint8_t)(MAX485_PIN);
    MODBUS_CHECK(&_is_MAX485_free, MODBUS_DEFAULT_DELAY);

    uart1_state = UART1_SEND;
    uart_tx_data(modbus_data.data, modbus_data.length);
    DELAY_MS(3);
    MODBUS_CHECK(&_is_modbus_txe, MODBUS_DEFAULT_DELAY);

    uart1_state = UART1_RECIEVE;
    GPIOD->ODR &= ~(uint8_t)(MAX485_PIN);
    MODBUS_CHECK(&_is_MAX485_free, MODBUS_DEFAULT_DELAY);
}

void _clear_data()
{
    for (uint8_t i = 0; i < sizeof(modbus_data.data); i++) {
        modbus_data.data[i] = 0;
    }
    modbus_data.length = 0;
    modbus_data.start_time = Global_time;
}

bool _is_MAX485_ready()
{
    return !(GPIOD->IDR & RX_PIN);
}

bool _is_modbus_txe()
{
    return UART1->SR & UART1_SR_TXE;
}

bool _is_MAX485_free()
{
    return !_is_MAX485_ready();
}