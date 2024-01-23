#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>
#include <stdbool.h>


typedef struct _timer_t {
	uint32_t start;
	uint32_t delay;
} timer_t;


void timer_start(timer_t* tm, uint32_t waitMs);
bool is_timer_wait(timer_t* tm);

bool wait_event(bool (*condition) (void), uint32_t time);
uint32_t get_clock_freq();
void delay_ms(uint32_t time);
uint32_t abs(int32_t number);
uint32_t u32_pow(uint32_t number, uint32_t degree);
uint8_t get_crc8(uint8_t* buffer, uint16_t size);


#endif