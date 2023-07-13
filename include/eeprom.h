#ifndef _EEPROM_H_
#define _EEPROM_H_


#include <stdint.h>


void eeprom_write(uint16_t addr, uint8_t *buf, uint16_t len);
void eeprom_read(uint16_t addr, uint8_t *buf, int len);


#endif