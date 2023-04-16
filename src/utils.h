#ifndef _UTILS_H_
#define _UTILS_H_


#define ABS(first_n, second_n) (first_n > second_n) ? first_n - second_n : second_n - first_n

#define ABS_DIF(first_n, second_n) ABS(first_n, second_n)

#define CONCAT(a, b)	a ## -> ## b

#define PORT(a, b)		CONCAT(a , b)


#endif