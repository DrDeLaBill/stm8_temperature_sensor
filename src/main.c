#include "main.h"

#include <string.h>

#include "interrupts.h"
#include "modbus/mb.h"
#include "modbus/mb-table.h"
#include "i2c.h"
#include "adt7420.h"


const uint8_t HSIDivFactor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */


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
    adt7420_init();
    
    mb_slave_address_set(SLAVE_DEVICE_ID);
    mb_set_tx_handler(&Uart1_SendNByte);

    mb_table_write(TABLE_Holding_Registers, 0, 123);

    enableInterrupts();

    int16_t tempr;
    while (1) {
      // tempr = 0;
      // adt7420_get_temperature(&tempr);
      // mb_table_write(TABLE_Holding_Registers, 0, tempr);
    }

    return 0;
}

static void SystemClockInit(void)
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

static void GPIOInit(void)
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

static void UART1Init(void)
{
  PD_DDR |= (uint8_t)(TX_PIN);
  PD_DDR &= (uint8_t)(~RX_PIN);
  PD_DDR |= (uint8_t)(MAX485_PIN);
  PD_CR1 |= (uint8_t)(MAX485_PIN);
  PD_CR2 |= (uint8_t)(MAX485_PIN);

  PD_ODR &= (uint8_t)(~MAX485_PIN);

  uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
  uint32_t BaudRate = UART_BAUD_RATE;

 /* Clear the word length bit */
  UART1_CR1 &= (uint8_t)(~UART1_CR1_M);  
  
  /* Set the word length bit according to UART1_WordLength value */
  UART1_CR1 |= (uint8_t)UART_WORDLENGTH;
  
  /* Clear the STOP bits */
  UART1_CR3 &= (uint8_t)(~UART1_CR3_STOP);  
  /* Set the STOP bits number according to UART1_StopBits value  */
  UART1_CR3 |= (uint8_t)UART_STOP_BITS;  
  
  /* Clear the Parity Control bit */
  UART1_CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));  
  /* Set the Parity Control bit to UART1_Parity value */
  UART1_CR1 |= (uint8_t)UART_PARITY;  
  
  /* Clear the LSB mantissa of UART1DIV  */
  UART1_BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);  
  /* Clear the MSB mantissa of UART1DIV  */
  UART1_BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);  
  /* Clear the Fraction bits of UART1DIV */
  UART1_BRR2 &= (uint8_t)(~UART1_BRR2_DIVF);  
  
  /* Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
  BaudRate_Mantissa    = ((uint32_t)get_clock_freq() / (BaudRate << 4));
  BaudRate_Mantissa100 = (((uint32_t)get_clock_freq() * 100) / (BaudRate << 4));
  /* Set the fraction of UART1DIV  */
  UART1_BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
  /* Set the MSB mantissa of UART1DIV  */
  UART1_BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
  /* Set the LSB mantissa of UART1DIV  */
  UART1_BRR1 |= (uint8_t)BaudRate_Mantissa;           
  
  /* Disable the Transmitter and Receiver before setting the LBCL, CPOL and CPHA bits */
  UART1_CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
  /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1_CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL); 
  /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
  UART1_CR3 |= (uint8_t)((uint8_t)UART_SYNC_MODE & (uint8_t)(UART1_CR3_CPOL | 
                                                        UART1_CR3_CPHA | UART1_CR3_LBCL));  
  
  /* Set the Transmitter Enable bit */
  UART1_CR2 |= (uint8_t)UART1_CR2_TEN;
  /* Set the Receiver Enable bit */
  UART1_CR2 |= (uint8_t)UART1_CR2_REN;

  /* Clear the Clock Enable bit */
  UART1_CR3 &= (uint8_t)(~UART1_CR3_CKEN); 


  // PXNE interrupt
  UART1_CR2 |= (uint8_t)UART1_CR2_RIEN;

  // UART enable
  UART1_CR1 &= (uint8_t)(~UART1_CR1_UARTD);
}

uint32_t get_clock_freq()
{
  uint32_t clockfrequency = 0;
  uint8_t clocksource;
  uint8_t tmp = 0, presc = 0;
  
  /* Get CLK source. */
  clocksource = (uint8_t)CLK_CMSR;
  
  tmp = (uint8_t)(CLK_CKDIVR & CLK_CKDIVR_HSIDIV);
  tmp = (uint8_t)(tmp >> 3);
  presc = HSIDivFactor[tmp];
  clockfrequency = HSI_VALUE / presc;

  return((uint32_t)clockfrequency);
}

void Uart1_Putchar(uint8_t Ch)
{

  PD_ODR |= (uint8_t)(MAX485_PIN);

  UART1_DR = Ch;
  while(!(UART1_SR & 0x80));

  PD_ODR &= (uint8_t)(~MAX485_PIN);
}

void Uart1_SendNByte(uint8_t* Data,uint8_t Len)
{
  while(Len--) Uart1_Putchar(*Data++);
}

INTERRUPT_DEFINITION(UART1_RX_IRQHandler, 18)
{
  mb_rx_new_data((uint8_t)UART1_DR);
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