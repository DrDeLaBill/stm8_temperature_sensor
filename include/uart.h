#ifndef _UART_H_
#define _UART_H_


#include <stdint.h>


void uart_init(uint32_t baud_rate, uint32_t f_master);
void uart_tx_byte(uint8_t data);
void uart_tx_data(uint8_t * data, uint8_t len);
uint8_t uart_rx_byte();
void uart_rx_data(uint8_t * data, uint8_t len);


#endif