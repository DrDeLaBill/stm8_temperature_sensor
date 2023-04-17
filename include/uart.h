#ifndef _UART_H_
#define _UART_H_


#include <stdint.h>


typedef enum {
    UART1_RECIEVE = 0x00,
    UART1_SEND
} UART1_STATE_Typedef;


extern UART1_STATE_Typedef uart1_state;


void uart_init(uint32_t baud_rate, uint32_t f_master);
void uart_tx_byte(uint8_t data);
void uart_tx_data(uint8_t * data, uint8_t len);
void uart_rx_data(uint8_t * data, uint8_t len);


#endif