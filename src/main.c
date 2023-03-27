#include "main.h"

#include <string.h>
// #include <stdio.h>

#include "stm8s.h"
#include "stm8s_it.h"
#include "mb.h"
#include "mb-table.h"
#include "i2c.h"
#include "adt7420.h"


CONST uint8_t HSIDivFactor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */


static void SystemClockInit(void);
static void GPIOInit(void);
static void UART1Init(void);

void Uart1_Putchar(uint8_t Ch);
void Uart1_SendNByte(uint8_t* Data,uint8_t Len);


int main(void) 
{
    SystemClockInit();
    GPIOInit();
    UART1Init();
    i2c_master_init(F_MASTER_HZ, F_I2C_HZ);

    uint8_t tmp[1] = {0};
    if (adt7420_init() != ADT7420_SUCCESS) {
      // while (1);
      tmp[0] = 0xFF;
    }
    
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&Uart1_SendNByte);

    // mb_table_write(TABLE_Holding_Registers, 0, 123);

    enableInterrupts();
    while (1) {
      Uart1_SendNByte(tmp, 1);
      // sprintf(tmp, "%d\n", ADT7420_get_tempr());
      // Uart1_SendNByte(tmp, strlen(tmp));
    }

    return 0;
}

static void SystemClockInit(void)
{
  // CLK deinit
  CLK->ICKR = CLK_ICKR_RESET_VALUE;
  CLK->ECKR = CLK_ECKR_RESET_VALUE;
  CLK->SWR  = CLK_SWR_RESET_VALUE;
  CLK->SWCR = CLK_SWCR_RESET_VALUE;
  CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
  CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
  CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
  CLK->CSSR = CLK_CSSR_RESET_VALUE;
  CLK->CCOR = CLK_CCOR_RESET_VALUE;
  while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0);
  CLK->CCOR = CLK_CCOR_RESET_VALUE;
  CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
  CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;

  // HSI & CPU prescaler
  CLK->CKDIVR |= (uint8_t)(SENSOR_CLK_DIVIDER) ;
  // Enable periph tick
  CLK->PCKENR1 |= (uint8_t)(CLK_PCKENR1_UART1);
  CLK->PCKENR1 |= (uint8_t)(CLK_PCKENR1_I2C);
  // HSI enable
  CLK->ICKR |= (uint8_t)(CLK_ICKR_HSIEN);
  while((CLK->ICKR & CLK_ICKR_HSIRDY) == 0);
}

static void GPIOInit(void)
{
  GPIOD->ODR = GPIO_ODR_RESET_VALUE;
  GPIOD->DDR = GPIO_DDR_RESET_VALUE;
  GPIOD->CR1 = GPIO_CR1_RESET_VALUE;
  GPIOD->CR2 = GPIO_CR2_RESET_VALUE;

  GPIOB->ODR = GPIO_ODR_RESET_VALUE;
  GPIOB->DDR = GPIO_DDR_RESET_VALUE;
  GPIOB->CR1 = GPIO_CR1_RESET_VALUE;
  GPIOB->CR2 = GPIO_CR2_RESET_VALUE;
}

static void UART1Init(void)
{
  GPIOD->DDR |= (uint8_t)(TX_PIN);
  GPIOD->DDR &= (uint8_t)(~RX_PIN);
  GPIOD->DDR |= (uint8_t)(MAX485_PIN);
  GPIOD->CR1 |= (uint8_t)(MAX485_PIN);
  GPIOD->CR2 |= (uint8_t)(MAX485_PIN);

  GPIOD->ODR &= (uint8_t)(~MAX485_PIN);

  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
  uint32_t BaudRate = UART_BAUD_RATE;

 /* Clear the word length bit */
  UART1->CR1 &= (uint8_t)(~UART1_CR1_M);  
  
  /* Set the word length bit according to UART1_WordLength value */
  UART1->CR1 |= (uint8_t)UART_WORDLENGTH;
  
  /* Clear the STOP bits */
  UART1->CR3 &= (uint8_t)(~UART1_CR3_STOP);  
  /* Set the STOP bits number according to UART1_StopBits value  */
  UART1->CR3 |= (uint8_t)UART_STOP_BITS;  
  
  /* Clear the Parity Control bit */
  UART1->CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));  
  /* Set the Parity Control bit to UART1_Parity value */
  UART1->CR1 |= (uint8_t)UART_PARITY;  
  
  /* Clear the LSB mantissa of UART1DIV  */
  UART1->BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);  
  /* Clear the MSB mantissa of UART1DIV  */
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);  
  /* Clear the Fraction bits of UART1DIV */
  UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVF);  
  
  /* Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
  BaudRate_Mantissa    = ((uint32_t)get_clock_freq() / (BaudRate << 4));
  BaudRate_Mantissa100 = (((uint32_t)get_clock_freq() * 100) / (BaudRate << 4));
  /* Set the fraction of UART1DIV  */
  UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
  /* Set the MSB mantissa of UART1DIV  */
  UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
  /* Set the LSB mantissa of UART1DIV  */
  UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;           
  
  /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
  UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
  /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1->CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL); 
  /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1->CR3 |= (uint8_t)((uint8_t)UART_SYNC_MODE & (uint8_t)(UART1_CR3_CPOL | 
                                                        UART1_CR3_CPHA | UART1_CR3_LBCL));  
  
  /* Set the Transmitter Enable bit */
  UART1->CR2 |= (uint8_t)UART1_CR2_TEN;
  /* Set the Receiver Enable bit */
  UART1->CR2 |= (uint8_t)UART1_CR2_REN;

  /* Clear the Clock Enable bit */
  UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN); 


  // PXNE interrupt
  UART1->CR2 |= (uint8_t)UART1_CR2_RIEN;

  // UART enable
  UART1->CR1 &= (uint8_t)(~UART1_CR1_UARTD);
}

uint32_t get_clock_freq()
{
  uint32_t clockfrequency = 0;
  uint8_t clocksource;
  uint8_t tmp = 0, presc = 0;
  
  /* Get CLK source. */
  clocksource = (uint8_t)CLK->CMSR;
  
  tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
  tmp = (uint8_t)(tmp >> 3);
  presc = HSIDivFactor[tmp];
  clockfrequency = HSI_VALUE / presc;

  return((uint32_t)clockfrequency);
}

void Uart1_Putchar(uint8_t Ch)
{

  GPIOD->ODR |= (uint8_t)(MAX485_PIN);

  UART1->DR = Ch;
  while(!(UART1->SR & 0x80));

  GPIOD->ODR &= (uint8_t)(~MAX485_PIN);
}

void Uart1_SendNByte(uint8_t* Data,uint8_t Len)
{
  while(Len--) Uart1_Putchar(*Data++);
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

  /* Infinite loop */
  while (1) {

  }
}

#endif