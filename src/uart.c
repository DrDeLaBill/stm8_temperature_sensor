#include "uart.h"

#include <stdint.h>

#include "main.h"
#include "interrupts.h"
#include "modbus/mb.h"


void uart1_init()
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

void uart1_putchar(uint8_t Ch)
{
  UART1_DR = Ch;
  while(!(UART1_SR & 0x80));
}

void uart1_send_byte(uint8_t* Data,uint8_t Len)
{
  PD_ODR |= (uint8_t)(MAX485_PIN);

  while(Len--) uart1_putchar(*Data++);

  PD_ODR &= (uint8_t)(~MAX485_PIN);
}

INTERRUPT_DEFINITION(UART1_RX_IRQHandler, 18)
{
  mb_rx_new_data((uint8_t)UART1_DR);
}