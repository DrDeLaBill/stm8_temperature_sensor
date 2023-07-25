#include "main.h"

#include "stm8s.h"
#include "stm8s_it.h"

#include "tim.h"
#include "utils.h"
#include "adt7420.h"
#include "modbus_manager.h"
#include "settings.h"


void system_clock_init();
void gpio_init();
bool _check_sensor_need_sleep();
void _enable_periph_clk();


volatile uint32_t global_time_ms = 0;
volatile bool is_sensor_sleep = false;


int main(void)
{
    system_clock_init();
    tim_init();
    gpio_init();
    adt7420_init();

    if (!sttngs_load()) {
      sttngs_reset();
    }
    
    modbus_manager_init();

    enableInterrupts();

    while (1) {
      if (is_modbus_busy()) {
        modbus_proccess();
        continue;
      }

      adt7420_proccess();

      if (_check_sensor_need_sleep()) {
        sensor_sleep();
      }
    }

    return 0;
}

void sensor_sleep() 
{
  is_sensor_sleep = true;
  CLK->PCKENR1 &= ~(uint8_t)(
    ((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_TIMER1 & (uint8_t)0x0F)) |
    ((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_I2C & (uint8_t)0x0F))
  );
  wfi();
}

void sensor_wake_up()
{
  is_sensor_sleep = false;
  adt7420_enable_sensor();
  _enable_periph_clk();
}

bool _check_sensor_need_sleep()
{
  return adt7420_is_measurments_done() && !is_modbus_busy();
}

void _enable_periph_clk() 
{
  CLK->PCKENR2 = 0x00;
  CLK->PCKENR1 |= ((uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_UART1 & (uint8_t)0x0F))
               |  (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_I2C & (uint8_t)0x0F))
               |  (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_TIMER1 & (uint8_t)0x0F))
               |  (uint8_t)((uint8_t)1 << ((uint8_t)CLK_PERIPHERAL_TIMER2 & (uint8_t)0x0F)));
}

void system_clock_init()
{
  CLK->ICKR &= 0x00;
  CLK->ICKR |= CLK_ICKR_HSI_EN;

  while(!(CLK->ICKR & CLK_ICKR_HSI_RDY));

  /* Clear High speed internal clock prescaler */
  CLK->CKDIVR &= 0x00;
  /* Set High speed internal clock prescaler */
  CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV;
  CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_CPUDIV;

  _enable_periph_clk();
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
  disableInterrupts();
  /* Infinite loop */
  while (1)
  {
  }
}
#endif