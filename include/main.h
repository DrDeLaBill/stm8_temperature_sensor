#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm8s.h"


// Sensor
#define SENSOR_VERSION        0x01
#define DEFAULT_DEVICE_ID     0x01
#define TEMPERATURE_REGISTER  0x0000 // output register
#define VERSION_REGISTER      0x0001 // output register
#define SLAVE_ID_REGISTER     0x0000 // input register
#define MAX_SLAVE_ID          0x80
// CLK
#define CLK_ICKR_HSI_EN       ((uint8_t)0x01)
#define CLK_ICKR_HSI_RDY      ((uint8_t)0x02)
#define CLK_PRESCALER_HSIDIV  ((uint8_t)0x18)
#define CLK_PRESCALER_CPUDIV  ((uint8_t)0x01) /*!< CPU clock division factors 1 */
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
#define CLK_PERIPHERAL_TIMER2  (uint8_t)0x05 /*!< Peripheral Clock Enable 1, Timer1 */
// TIM1
#define TIM1_PRESCALER            ((uint16_t)1000)
#define TIM1_PERIOD               ((uint16_t)10)
#define TIM1_REP_COUNTER          ((uint8_t)0x00)
#define TIM1_COUNTERMODE_DOWN     ((uint8_t)0x10)
#define TIM1_PSCRELOADMODE_UPDATE ((uint8_t)0x00)
#define TIM1_IT_UPDATE            ((uint8_t)0x01)
#define TIM1_IT_CC1               ((uint8_t)0x02)

#define TIM1_CR1_CMS              ((uint8_t)0x60) /*!< Center-aligned Mode Selection mask. */
#define TIM1_CR1_DIR              ((uint8_t)0x10) /*!< Direction mask. */
// TIM2
#define TIM2_PRESCALER_256        ((uint8_t)0x08)
#define TIM2_PRESCALER            TIM2_PRESCALER_256
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
#define I2C_ADT7420_ADDR   ((uint8_t)(0x48 << 1 | 0))
#define SET16BITMODE       0b10100000
#define F_I2C_HZ           100000UL
// ADT7420
#define ADT7420_DELAY            ((uint32_t)1000)
#define ADT7420_MEASURE_TIME     ((uint32_t)5000)
// MODBUS
#define MODBUS_TIMEOUT_MS        ((uint32_t)100)
// IWDG
#define IWDG_KEY_ENABLE          ((uint8_t)0xCC) /*!<  This value written in the Key register start the watchdog counting down*/
#define IWDG_WriteAccess_Enable  ((uint8_t)0x55) /*!< Code 0x55 in Key register, allow write access to Prescaler and Reload registers */
#define IWDG_Prescaler_4         ((uint8_t)0x00) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_8         ((uint8_t)0x01) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_16        ((uint8_t)0x02) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_32        ((uint8_t)0x03) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_64        ((uint8_t)0x04) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_128       ((uint8_t)0x05) /*!< Used to set prescaler register to 32 */
#define IWDG_Prescaler_256       ((uint8_t)0x06) /*!< Used to set prescaler register to 32 */
#define IWDG_WriteAccess_Disable ((uint8_t)0x00) /*!< Code 0x00 in Key register, not allow write access to Prescaler and Reload registers */
#define IWDG_KEY_REFRESH         ((uint8_t)0xAA)  /*!<  This value written in the Key register prevent the watchdog reset */


extern volatile uint32_t global_time_ms;
extern volatile bool is_sensor_sleep;


void sensor_sleep();
void sensor_wake_up();


#endif