#include "uart.h"

#include <stdint.h>

#include "stm8s.h"
#include "main.h"
#include "utils.h"


#define UART_CHECK(condition, time)   if (!wait_event(condition, time)) {return;}
#define UART_CHECK_D(condition, time) if (!wait_event(condition, time)) {return 0;}


bool _is_uart_txe();
bool _is_uart_rxne();


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
  UART_CHECK(&_is_uart_txe, 5);
  UART1->DR = data;
}

//******************************************************************************
// Прием байта
//******************************************************************************
uint8_t uart_rx_byte() {
  uint8_t data;
  UART_CHECK_D(&_is_uart_rxne, 5);
  data = UART1->DR;
  return data;
}

//******************************************************************************
// Отправка массива данных
//******************************************************************************
void uart_tx_data(uint8_t * data, uint8_t len) {
  while (len--) {
    uart_tx_byte(*data++);
  }
}

//******************************************************************************
// Прием массива данных
//******************************************************************************
void uart_rx_data(uint8_t* data, uint8_t len) {
  while(len--) {
    *data++ = uart_rx_byte();
  }
}

bool _is_uart_txe()
{
  return UART1->SR & UART1_SR_TXE;
}

bool _is_uart_rxne()
{
  return UART1->SR & UART1_SR_RXNE;
}