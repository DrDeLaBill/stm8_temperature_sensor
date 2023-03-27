#ifndef _MAIN_H_
#define _MAIN_H_


#include "stm8s.h"


#define GPIO_PIN_4         ((uint8_t)0x10)
#define GPIO_PIN_5         ((uint8_t)0x20)
#define GPIO_PIN_6         ((uint8_t)0x40)

// Config
#define SENSOR_CLK_DIVIDER 0x00
// UART
#define UART_BAUD_RATE     (uint32_t)9600
#define UART_WORDLENGTH    0x00
#define UART_STOP_BITS     0x00
#define UART_PARITY        0x00
#define UART_SYNC_MODE     0x80
#define UART_MODE_TXRX     0x0C
#define TX_PIN             GPIO_PIN_5
#define RX_PIN             GPIO_PIN_6
#define MAX485_PIN         GPIO_PIN_4
// I2C
#define I2C_FREQ           I2C_MAX_STANDARD_FREQ
#define I2C_ACK            0x01
#define I2C_ADT7420_ADDR   0x4B
#define ADT7420TempReg     0x00
#define ADT7420ConfigReg   0x03
#define SET16BITMODE       0b10100000
#define F_I2C_HZ           100000UL
// Sensor
#define SLAVE_DEVICE_ID    0x01


uint32_t get_clock_freq();


#endif