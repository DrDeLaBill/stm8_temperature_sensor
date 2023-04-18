#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>

#include "stm8s.h"


#define ABS(number)                (uint32_t)(((number) < 0) ? (-1 * (number)) : (number))

#define ABS_DIF(first_n, second_n) (uint32_t)(((first_n) > (second_n)) ? ((first_n) - (second_n)) : ((second_n) - (first_n)))

#define DELAY_MS(time_ms)          uint32_t start_time = Global_time; while (ABS_DIF(start_time, Global_time) < time_ms);


bool wait_event(bool (*condition) (void), uint32_t time);
uint32_t get_clock_freq();


#endif