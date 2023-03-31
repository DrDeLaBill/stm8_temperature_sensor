#ifndef _UART_H_
#define _UART_H_


#include <stdint.h>


void uart1_init();
void uart1_putchar(uint8_t Ch);
void uart1_send_byte(uint8_t* Data,uint8_t Len);


#endif