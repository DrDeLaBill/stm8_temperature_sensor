#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>

#include "stm8s.h"


bool wait_event(bool (*condition) (void), uint32_t time);
uint32_t get_clock_freq();
void delay_ms(uint32_t time);
int32_t abs_dif(int32_t first_n, int32_t second_n);
int32_t abs(int32_t number);


#endif