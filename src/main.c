#include "main.h"

#include <string.h>

#include "uart.h"
#include "utils.h"
#include "adt7420.h"
#include "modbus/mb.h"
#include "modbus/mb-table.h"


const uint8_t HSIDivFactor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */

uint32_t Global_time   = 0; // global time in ms
uint32_t mb_start_time = 0; // modbus delay


void system_clock_init();
void gpio_init();
void tim2_init();


int main(void) 
{
    enableInterrupts();
    system_clock_init();
    tim2_init();
    gpio_init();
    uart1_init();
    adt7420_init();

    
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&uart1_send_byte);

    int16_t tempr;
    uint8_t reg_counter = 0;
    mb_start_time = Global_time;
    for (uint8_t i = 0; i < TABLE_Holding_Registers_Size; i++) {
      mb_table_write(TABLE_Holding_Registers, i, 100 + i);
    }
    uint8_t i = 0;
    char str[2] = "1\n";
    while (1) {
      if (ABS_DIF(Global_time, mb_start_time) > MODBUS_DELAY) {
        mb_rx_timeout_handler();
      }
      str[0] = '0' + (char)Global_time;
      uart1_send_byte(str, 2);
      // tempr = 0;
      // adt7420_get_temperature(&tempr);
      // mb_table_write(TABLE_Holding_Registers, reg_counter++, tempr);
      // if (reg_counter >= TABLE_Holding_Registers_Size) {
      //   reg_counter = 0;
      // }
    }

    return 0;
}

void system_clock_init()
{
  STM8S103;
  // CLK deinit
  CLK_ICKR = CLK_ICKR_RESET_VALUE;
  CLK_ECKR = CLK_ECKR_RESET_VALUE;
  CLK_SWR  = CLK_SWR_RESET_VALUE;
  CLK_SWCR = CLK_SWCR_RESET_VALUE;
  CLK_CKDIVR = CLK_CKDIVR_RESET_VALUE;
  CLK_SPCKENR1 = CLK_PCKENR1_RESET_VALUE;
  CLK_PCKENR2 = CLK_PCKENR2_RESET_VALUE;
  CLK_CSSR = CLK_CSSR_RESET_VALUE;
  CLK_CCOR = CLK_CCOR_RESET_VALUE;
  while ((CLK_CCOR & CLK_CCOR_CCOEN)!= 0);
  CLK_CCOR = CLK_CCOR_RESET_VALUE;
  CLK_HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
  CLK_SWIMCCR = CLK_SWIMCCR_RESET_VALUE;

  // HSI & CPU prescaler
  CLK_CKDIVR |= (uint8_t)(SENSOR_CLK_DIVIDER);
  // Enable periph tick
  CLK_SPCKENR1 |= (uint8_t)(CLK_PCKENR1_UART1);
  CLK_SPCKENR1 |= (uint8_t)(CLK_PCKENR1_I2C);
  CLK_SPCKENR1 |= (uint8_t)(CLK_PCKENR1_TIM1);

  // HSI enable
  CLK_ICKR |= (uint8_t)(CLK_ICKR_HSIEN);
  while((CLK_ICKR & CLK_ICKR_HSIRDY) == 0);

  CLK_SWCR |= CLK_SWCR_SWEN;
  while (CLK_SWCR & CLK_SWCR_SWBSY != 0);
}

void gpio_init()
{
  PD_ODR = GPIO_ODR_RESET_VALUE;
  PD_DDR = GPIO_DDR_RESET_VALUE;
  PD_CR1 = GPIO_CR1_RESET_VALUE;
  PD_CR2 = GPIO_CR2_RESET_VALUE;

  PB_ODR = GPIO_ODR_RESET_VALUE;
  PB_DDR = GPIO_DDR_RESET_VALUE;
  PB_CR1 = GPIO_CR1_RESET_VALUE;
  PB_CR2 = GPIO_CR2_RESET_VALUE;
}

void tim2_init()
{
  // TIM1 - system timer (1ms)
	TIM3_PSCR = 0x04; // LSB should be written last as it updates prescaler
	// auto-reload each 1ms: TIM_ARR = 1000 = 0x03E8
	TIM2_ARRH = 0x03;
	TIM2_ARRL = 0xE8;
	// interrupts: update
	TIM2_IER = TIM_IER_UIE;
	// auto-reload + interrupt on overflow + enable
	TIM2_CR1 = TIM_CR1_APRE | TIM_CR1_URS | TIM_CR1_CEN;

  // TIM3_PSCR = 0x04;
  // TIM3_ARRH = 0x03;
  // TIM3_ARRL = 0xE8;
  // TIM3_IER = TIM_IER_UIE;
  // TIM3_CR1 = TIM_CR1_APRE | TIM_CR1_CEN;
}

uint32_t get_clock_freq()
{
  uint32_t clock_frequency = 0;
  uint8_t clock_source;
  uint8_t tmp = 0, presc = 0;
  
  /* Get CLK source. */
  clock_source = (uint8_t)CLK_CMSR;
  
  tmp = (uint8_t)(CLK_CKDIVR & CLK_CKDIVR_HSIDIV);
  tmp = (uint8_t)(tmp >> 3);
  presc = HSIDivFactor[tmp];
  clock_frequency = HSI_VALUE / presc;

  return ((uint32_t)clock_frequency);
}
