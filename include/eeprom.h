#ifndef _EEPROM_H_
#define _EEPROM_H_


#include <stdint.h>
#include <stdbool.h>


#define _MEM_(mem_addr)   (*(volatile uint8_t *)(mem_addr))
#define EEPROM_START_ADDR 0x4010
#define EEPROM_END_ADDR   0x407F


bool eeprom_write(uint16_t addr, uint8_t *buf, uint16_t len);
void eeprom_read(uint16_t addr, uint8_t *buf, int len);


#endif