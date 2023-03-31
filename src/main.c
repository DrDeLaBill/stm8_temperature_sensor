#include "main.h"

#include <string.h>

#include "uart.h"
#include "adt7420.h"
#include "modbus/mb.h"
#include "modbus/mb-table.h"


const uint8_t HSIDivFactor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */


void system_clock_init();
void gpio_init();


int main(void) 
{
    system_clock_init();
    gpio_init();
    uart1_init();
    adt7420_init();
    
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&uart1_send_byte);

    enableInterrupts();

    int16_t tempr;
    uint8_t reg_counter = 0;
    while (1) {
      tempr = 0;
      adt7420_get_temperature(&tempr);
      mb_table_write(TABLE_Holding_Registers, reg_counter++, tempr);
      if (reg_counter >= TABLE_Holding_Registers_Size) {
        reg_counter = 0;
      }
    }

    return 0;
}

void system_clock_init()
{
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
  CLK_CKDIVR |= (uint8_t)(SENSOR_CLK_DIVIDER) ;
  // Enable periph tick
  CLK_SPCKENR1 |= (uint8_t)(CLK_PCKENR1_UART1);
  CLK_SPCKENR1 |= (uint8_t)(CLK_PCKENR1_I2C);
  // HSI enable
  CLK_ICKR |= (uint8_t)(CLK_ICKR_HSIEN);
  while((CLK_ICKR & CLK_ICKR_HSIRDY) == 0);
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