#include "modbus_manager.h"

#include "main.h"
#include "mb.h"
#include "mb-table.h"
#include "uart.h"


void _modbus_data_handler(uint8_t * data, uint8_t len);
void _clear_data();


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
    GPIOD->ODR |= (uint8_t)(MAX485_PIN);
    while (GPIOD->IDR & RX_PIN);
    uart1_state = UART1_SEND;
    uart_tx_data(modbus_data.data, modbus_data.length);
    uart_tx_byte(0);
    while(!(UART1->SR & UART1_SR_TXE));
    uart1_state = UART1_RECIEVE;
    GPIOD->ODR &= ~(uint8_t)(MAX485_PIN);
    while (GPIOD->IDR & MAX485_PIN);
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

void _clear_data()
{
    for (uint8_t i = 0; i < sizeof(modbus_data.data); i++) {
        modbus_data.data[i] = 0;
    }
    modbus_data.length = 0;
}