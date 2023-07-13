#include "eeprom.h"

#include "stm8s_flash.h"
#include "stm8s.h"
#include "main.h"


#define FLASH_RASS_KEY1 ((uint8_t)0x56) /*!< First RASS key */
#define FLASH_RASS_KEY2 ((uint8_t)0xAE) /*!< Second RASS key */


void eeprom_write(uint16_t addr, uint8_t *buf, uint16_t len) {
    /* unlock EEPROM */
    FLASH->DUKR = FLASH_RASS_KEY1;
    FLASH->DUKR = FLASH_RASS_KEY2;
    while (!(FLASH->IAPSR & (1 << FLASH_IAPSR_DUL)));

    /* write data from buffer */
    for (uint16_t i = 0; i < len; i++, addr++) {
        _MEM_(addr) = buf[i];
        while (!(FLASH->IAPSR & (1 << FLASH_IAPSR_EOP)));
    }

    /* lock EEPROM */
    FLASH->IAPSR &= ~(1 << FLASH_IAPSR_DUL);
}

void eeprom_read(uint16_t addr, uint8_t *buf, int len) {
    /* read EEPROM data into buffer */
    for (int i = 0; i < len; i++, addr++) {
        buf[i] = _MEM_(addr);
    }
}