#include "uart.h"

#include <stdint.h>

#include "stm8s.h"
#include "main.h"


UART1_STATE_Typedef uart1_state = UART1_RECIEVE;


void uart_init(uint32_t baud_rate, uint32_t f_master) {
  
  //Значение регистра BRR
  unsigned long brr;
  
  //Настраиваем TX на выход, а RX на вход
  GPIOD->DDR |= GPIO_PIN_5; // tx
  GPIOD->DDR &= ~GPIO_PIN_6; // rx
  
  //RX - плавающий вход
  GPIOD->CR1 &= ~GPIO_PIN_6;
  //Отключает внешние прерывания для RX
  GPIOD->CR2 &= ~GPIO_PIN_6;
  
  //Настройка скорости передачи
  brr = f_master/baud_rate;
    
  UART1->BRR2 = brr & 0x000F;
  UART1->BRR2 |= brr >> 12;
  UART1->BRR1 = (brr >> 4) & 0x00FF;
  
  //Четность отключена
  UART1->CR1 &= ~UART1_CR1_PIEN;
  //Контроль четности отключен
  UART1->CR1 &= ~UART1_CR1_PCEN;
  //8-битный режим
  UART1->CR1 &= ~UART1_CR1_M;
  //Включить UART
  UART1->CR1 &= ~UART1_CR1_UARTD;
  
  //Запретить прерывание по опустошению передающ. регистра
  UART1->CR2 &= ~UART1_CR2_TIEN;
  //Запретить прерывание по завершению передачи
  UART1->CR2 &= ~UART1_CR2_TCIEN;
  //Разрешить прерывание по заполнению приемного регистра
  UART1->CR2 |= UART1_CR2_RIEN;
  //Запретить прерывание по освобождению линии
  UART1->CR2 &= ~UART1_CR2_ILIEN;
  //Передатчик включить
  UART1->CR2 |= UART1_CR2_TEN;
  //Приемник включить
  UART1->CR2 |= UART1_CR2_REN;
  //Не посылать break-символ
  UART1->CR2 &= ~UART1_CR2_SBK;
  
  //Один стоп-бит
  UART1->CR3 &= ~UART1_CR3_STOP;
}

//******************************************************************************
// Отправка байта
//******************************************************************************
void uart_tx_byte(uint8_t data) {
  while(!(UART1->SR & UART1_SR_TXE));
  UART1->DR = data;
}

//******************************************************************************
// Прием байта
//******************************************************************************
uint8_t uart_rx_byte() {
  uint8_t data;
  while(!(UART1->SR & UART1_SR_RXNE));
  data = UART1->DR;
  return data;
}

//******************************************************************************
// Отправка массива данных
//******************************************************************************
void uart_tx_data(uint8_t * data, uint8_t len) {
  GPIOD->ODR |= (uint8_t)(MAX485_PIN);
  while (GPIOD->IDR & RX_PIN);
  uart1_state = UART1_SEND;
  while (len--) {
    uart_tx_byte(*data++);
  }
  uart_tx_byte('\n');
  while(!(UART1->SR & UART1_SR_TXE));
  uart1_state = UART1_RECIEVE;
  GPIOD->ODR &= ~(uint8_t)(MAX485_PIN);
  while (GPIOD->IDR & MAX485_PIN);
}

//******************************************************************************
// Прием массива данных
//******************************************************************************
void uart_rx_data(uint8_t * data, uint8_t len) {
  while(len--){
    *data++ = uart_rx_byte();
  }
}