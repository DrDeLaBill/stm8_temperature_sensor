#include "utils.h"

#include <stdint.h>
#include <stdbool.h>

#include "main.h"


void timer_start(timer_t* tm, uint32_t waitMs)
{
    tm->start = global_time_ms;
    tm->delay = waitMs;
}

bool is_timer_wait(timer_t* tm)
{
    return abs(global_time_ms - tm->start) < tm->delay;
}

bool wait_event(bool (*condition) (void), uint32_t time)
{
    timer_t timer;
    timer_start(&timer, time);
    while (is_timer_wait(&timer)) {
        if (condition()) {
            return true;
        }
    }
    return false;
}

void delay_ms(uint32_t time) 
{
    timer_t timer;
    timer_start(&timer, time);
    while (is_timer_wait(&timer));
}

uint32_t abs_dif(int32_t first_n, int32_t second_n)
{
    return (int32_t)(((first_n) > (second_n)) ? ((first_n) - (second_n)) : ((second_n) - (first_n)));
}

uint32_t abs(int32_t number)
{
    return (int32_t)(((number) < 0) ? (-1 * (number)) : (number));
}

uint32_t u32_pow(uint32_t number, uint32_t degree)
{
  if (number % 2 == 0) {
    return number << degree;
  }
  int res = 1;
  while (degree--) {
    res *= number;
  }
  return res;
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

uint8_t get_crc8(uint8_t* buffer, uint16_t size) {
  uint8_t crc = 0;
  for (uint16_t i = 0; i < size; i++) {
    uint8_t data = buffer[i];
    for (uint16_t j = 8; j > 0; j--) {
      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
      data >>= 1;
    }
  }
  return crc;
}