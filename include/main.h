#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>

#include "stm8s.h"


// Config
#define F_MASTER_MHZ          16UL
#define F_MASTER_HZ           16000000UL
// Sensor
#define SLAVE_DEVICE_ID    0x01
// CLK
#define CLK_PRESCALER_HSIDIV1 (uint8_t)0x00
#define CLK_PRESCALER_CPUDIV1 (uint8_t)0x80, /*!< CPU clock division factors 1 */
#define CLK_CCOR_CCOEN        ((uint8_t)0x01) /*!< Configurable clock output enable */
#define CLK_PCKENR1_TIM1      ((uint8_t)0x80) /*!< Timer 1 clock enable */ 
#define CLK_PCKENR1_TIM2      ((uint8_t)0x20) /*!< Timer 2 clock enable */
#define CLK_PCKENR1_UART1     ((uint8_t)0x04) /*!< UART1 clock enable */
#define CLK_PCKENR1_I2C       ((uint8_t)0x01) /*!< I2C clock enable */
#define CLK_ICKR_HSIRDY       ((uint8_t)0x02) /*!< High speed internal RC oscillator ready */
#define CLK_CKDIVR_HSIDIV     ((uint8_t)0x18) /*!< High speed internal clock prescaler */
#define CLK_SWCR_SWEN         ((uint8_t)0x02) /*!< Switch start/stop */
#define CLK_SWCR_SWBSY        ((uint8_t)0x01) /*!< Switch busy flag*/
#define HSI_VALUE             ((uint32_t)16000000) /*!< Typical Value of the HSI in Hz */

#define CLK_PERIPHERAL_UART1   (uint8_t)0x03 /*!< Peripheral Clock Enable 1, UART1 */
#define CLK_PERIPHERAL_I2C     (uint8_t)0x00 /*!< Peripheral Clock Enable 1, I2C */
#define CLK_PERIPHERAL_TIMER1  (uint8_t)0x07 /*!< Peripheral Clock Enable 1, Timer1 */
// TIM1
#define TIM_PRESCALER             (uint16_t)1000
#define TIM_PERIOD                (uint16_t)160
#define TIM_REP_COUNTER           (uint8_t)0x00
#define TIM1_COUNTERMODE_DOWN     ((uint8_t)0x10)
#define TIM1_PSCRELOADMODE_UPDATE ((uint8_t)0x00)
#define TIM1_IT_UPDATE            ((uint8_t)0x01)
#define TIM1_IT_CC1               ((uint8_t)0x02)

#define TIM1_CR1_CMS     ((uint8_t)0x60) /*!< Center-aligned Mode Selection mask. */
#define TIM1_CR1_DIR     ((uint8_t)0x10) /*!< Direction mask. */

#define TIM1_CR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CR2_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SMCR_RESET_VALUE  ((uint8_t)0x00)
#define TIM1_ETR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_IER_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SR2_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_EGR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CCMR1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR3_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR4_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCER1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCER2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CNTRH_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CNTRL_RESET_VALUE ((uint8_t)0x00)
#define TIM1_PSCRH_RESET_VALUE ((uint8_t)0x00)
#define TIM1_PSCRL_RESET_VALUE ((uint8_t)0x00)
#define TIM1_ARRH_RESET_VALUE  ((uint8_t)0xFF)
#define TIM1_ARRL_RESET_VALUE  ((uint8_t)0xFF)
#define TIM1_RCR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CCR1H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR1L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_BKR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_DTR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_OISR_RESET_VALUE  ((uint8_t)0x00)
// GPIO
#define GPIO_ODR_RESET_VALUE ((uint8_t)0x00)
#define GPIO_DDR_RESET_VALUE ((uint8_t)0x00)
#define GPIO_CR1_RESET_VALUE ((uint8_t)0x00)
#define GPIO_CR2_RESET_VALUE ((uint8_t)0x00)
// UART1
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
#define UART1_BRR1_DIVM   ((uint8_t)0xFF) /*!< LSB mantissa of UART1DIV [7:0] mask */
#define UART1_BRR2_DIVM   ((uint8_t)0xF0) /*!< MSB mantissa of UART1DIV [11:8] mask */
#define UART1_BRR2_DIVF   ((uint8_t)0x0F) /*!< Fraction bits of UART1DIV [3:0] mask */
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
#define I2C_ADT7420_ADDR   (uint8_t)(0x48 << 1 | 0)
#define SET16BITMODE       0b10100000
#define F_I2C_HZ           100000UL
// ADT7420
#define ADT7420_DELAY      (uint32_t)1000
// MODBUS
#define MODBUS_WAIT_TIME   (uint32_t)1000
// IWDG
#define IWDG_KEY_ENABLE          ((uint8_t)0xCC) /*!<  This value written in the Key register start the watchdog counting down*/
#define IWDG_WriteAccess_Enable  ((uint8_t)0x55) /*!< Code 0x55 in Key register, allow write access to Prescaler and Reload registers */
#define IWDG_Prescaler_128       ((uint8_t)0x05) /*!< Used to set prescaler register to 128 */
#define IWDG_WriteAccess_Disable ((uint8_t)0x00) /*!< Code 0x00 in Key register, not allow write access to Prescaler and Reload registers */
#define IWDG_KEY_REFRESH         ((uint8_t)0xAA)  /*!<  This value written in the Key register prevent the watchdog reset */


uint32_t get_clock_freq();


extern volatile uint32_t Global_time;


#endif