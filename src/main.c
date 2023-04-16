#include "main.h"

#include "stm8s.h"
#include "stm8s_it.h"

#include "tim.h"
#include "uart.h"
#include "utils.h"
#include "adt7420.h"
#include "modbus/mb.h"
#include "modbus/mb-table.h"


void system_clock_init();
void gpio_init();
uint32_t get_clock_freq();


int main(void)
{
    system_clock_init();
    tim_init();
    gpio_init();
    uart_init(UART_BAUD_RATE, get_clock_freq());
    adt7420_init();

    enableInterrupts();
    
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&uart_tx_data);

    while (1) {
      adt7420_proccess();
    }

    return 0;
}

void  system_clock_init()
{
  CLK->ICKR |= ENABLE;
  
  /* Clear High speed internal clock prescaler */
  CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
  /* Set High speed internal clock prescaler */
  CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

  CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
  CLK->CKDIVR |= (uint8_t)((uint8_t)0x80 & (uint8_t)CLK_CKDIVR_CPUDIV);

  CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_UART1 & (uint8_t)0x0F));
  CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_TIMER1 & (uint8_t)0x0F));
  // CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_I2C & (uint8_t)0x0F));
}

void gpio_init()
{
  GPIOD->ODR = GPIO_ODR_RESET_VALUE;
  GPIOD->DDR = GPIO_DDR_RESET_VALUE;
  GPIOD->CR1 = GPIO_CR1_RESET_VALUE;
  GPIOD->CR2 = GPIO_CR2_RESET_VALUE;

  GPIOB->ODR = GPIO_ODR_RESET_VALUE;
  GPIOB->DDR = GPIO_DDR_RESET_VALUE;
  GPIOB->CR1 = GPIO_CR1_RESET_VALUE;
  GPIOB->CR2 = GPIO_CR2_RESET_VALUE;

  GPIOD->DDR |= (uint8_t)(TX_PIN);
  GPIOD->DDR &= (uint8_t)(~RX_PIN);

  GPIOD->DDR |= (uint8_t)(MAX485_PIN);
  GPIOD->CR1 |= (uint8_t)(MAX485_PIN);
  GPIOD->CR2 |= (uint8_t)(MAX485_PIN);

  GPIOD->ODR &= ~(uint8_t)(MAX485_PIN);
}

uint32_t get_clock_freq()
{
  uint32_t clock_frequency = 0;
  uint8_t clock_source;
  uint8_t tmp = 0, presc = 0;
  const uint8_t HSI_div_factor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */
  
  /* Get CLK source. */
  clock_source = (uint8_t)CLK->CMSR;
  
  tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
  tmp = (uint8_t)(tmp >> 3);
  presc = HSI_div_factor[tmp];
  clock_frequency = HSI_VALUE / presc;

  return ((uint32_t)clock_frequency);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  // disableInterrupts();
  /* Infinite loop */
  while (1)
  {
  }
}
#endif