#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdint.h>


#define ABS(number)                (uint32_t)(((number) < 0) ? (-1 * (number)) : (number))

#define ABS_DIF(first_n, second_n) (uint32_t)(((first_n) > (second_n)) ? ((first_n) - (second_n)) : ((second_n) - (first_n)))

#define CONCAT(a, b)	           a ## -> ## b

#define PORT(a, b)		           CONCAT(a , b)


#endif