#ifndef _MAIN_H_
#define _MAIN_H_

// Config
#define SENSOR_CLK_DIVIDER 0x00
#define UART_BAUD_RATE     (uint32_t)9600
#define UART_WORDLENGTH    0x00
#define UART_STOP_BITS     0x00
#define UART_PARITY        0x00
#define UART_SYNC_MODE     0x80
#define UART_MODE_TXRX     0x0C
#define TX_PIN             GPIO_PIN_5
#define RX_PIN             GPIO_PIN_6
#define MAX485_PIN         GPIO_PIN_4

// Sensor
#define SLAVE_DEVICE_ID    0x01

#endif