#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>

#include "stm8l.h"


#define GPIO_PIN_4         ((uint8_t)0x10)
#define GPIO_PIN_5         ((uint8_t)0x20)
#define GPIO_PIN_6         ((uint8_t)0x40)

// Config
#define F_MASTER_MHZ       16UL
#define F_MASTER_HZ        16000000UL
#define SENSOR_CLK_DIVIDER 0x00
// CLK
#define CLK_ICKR_RESET_VALUE     ((uint8_t)0x01)
#define CLK_ECKR_RESET_VALUE     ((uint8_t)0x00)
#define CLK_CMSR_RESET_VALUE     ((uint8_t)0xE1)
#define CLK_SWR_RESET_VALUE      ((uint8_t)0xE1)
#define CLK_SWCR_RESET_VALUE     ((uint8_t)0x00)
#define CLK_CKDIVR_RESET_VALUE   ((uint8_t)0x18)
#define CLK_PCKENR1_RESET_VALUE  ((uint8_t)0xFF)
#define CLK_PCKENR2_RESET_VALUE  ((uint8_t)0xFF)
#define CLK_CSSR_RESET_VALUE     ((uint8_t)0x00)
#define CLK_CCOR_RESET_VALUE     ((uint8_t)0x00)
#define CLK_HSITRIMR_RESET_VALUE ((uint8_t)0x00)
#define CLK_SWIMCCR_RESET_VALUE  ((uint8_t)0x00)

#define CLK_CCOR_CCOEN       ((uint8_t)0x01) /*!< Configurable clock output enable */
#define CLK_PCKENR1_UART1    ((uint8_t)0x04) /*!< UART1 clock enable */
#define CLK_PCKENR1_I2C      ((uint8_t)0x01) /*!< I2C clock enable */
#define CLK_ICKR_HSIRDY      ((uint8_t)0x02) /*!< High speed internal RC oscillator ready */
#define CLK_ICKR_HSIEN       ((uint8_t)0x01) /*!< High speed internal RC oscillator enable */
#define CLK_CKDIVR_HSIDIV    ((uint8_t)0x18) /*!< High speed internal clock prescaler */
#define HSI_VALUE            ((uint32_t)16000000) /*!< Typical Value of the HSI in Hz */
// GPIO
#define GPIO_ODR_RESET_VALUE ((uint8_t)0x00)
#define GPIO_DDR_RESET_VALUE ((uint8_t)0x00)
#define GPIO_CR1_RESET_VALUE ((uint8_t)0x00)
#define GPIO_CR2_RESET_VALUE ((uint8_t)0x00)
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

#define UART1_CR1_M       ((uint8_t)0x10) /*!< Word length mask */
#define UART1_CR3_STOP    ((uint8_t)0x30) /*!< STOP bits [1:0] mask */
#define UART1_CR1_PCEN    ((uint8_t)0x04) /*!< Parity Control Enable mask */
#define UART1_CR1_PS      ((uint8_t)0x02) /*!< UART1 Parity Selection */
#define UART1_BRR1_DIVM  ((uint8_t)0xFF) /*!< LSB mantissa of UART1DIV [7:0] mask */
#define UART1_BRR2_DIVM  ((uint8_t)0xF0) /*!< MSB mantissa of UART1DIV [11:8] mask */
#define UART1_BRR2_DIVF  ((uint8_t)0x0F) /*!< Fraction bits of UART1DIV [3:0] mask */
#define UART1_CR2_TEN     ((uint8_t)0x08) /*!< Transmitter Enable mask */
#define UART1_CR2_REN     ((uint8_t)0x04) /*!< Receiver Enable mask */
#define UART1_CR3_CPOL    ((uint8_t)0x04) /*!< Clock Polarity mask */
#define UART1_CR3_CPHA    ((uint8_t)0x02) /*!< Clock Phase mask */
#define UART1_CR3_LBCL    ((uint8_t)0x01) /*!< Last Bit Clock pulse mask */
#define UART1_CR3_CKEN    ((uint8_t)0x08) /*!< Clock Enable mask */
#define UART1_CR2_RIEN    ((uint8_t)0x20) /*!< Receiver Interrupt Enable mask */
#define UART1_CR1_UARTD   ((uint8_t)0x20) /*!< UART1 Disable (for low power consumption) */
// I2C
#define I2C_FREQ           I2C_MAX_STANDARD_FREQ
#define I2C_ACK            0x01
#define I2C_ADT7420_ADDR   0x48
#define SET16BITMODE       0b10100000
#define F_I2C_HZ           100000UL
// Sensor
#define SLAVE_DEVICE_ID    0x01


void uart1_send_byte(uint8_t* Data,uint8_t Len);
uint32_t get_clock_freq();


#endif