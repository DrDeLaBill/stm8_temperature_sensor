#include "eeprom.h"

#include <string.h>

#include "stm8s.h"
#include "main.h"
#include "utils.h"
#include "iwdg.h"


#define FLASH_RASS_KEY1 ((uint8_t)0x56) /*!< First RASS key */
#define FLASH_RASS_KEY2 ((uint8_t)0xAE) /*!< Second RASS key */

#define EEPROM_WAIT_WHILE(condition, time) if (!wait_event(condition, time)) {return FALSE;}
#define EEPROM_TIMEOUT 10


bool _if_IAPSR_DUL();
bool _if_IAPSR_EOP();


bool eeprom_write(uint16_t addr, uint8_t *buf, uint16_t len) {
    /* unlock EEPROM */
    FLASH->DUKR = FLASH_RASS_KEY2;
    FLASH->DUKR = FLASH_RASS_KEY1;

    iwdg_reload();
    EEPROM_WAIT_WHILE(&_if_IAPSR_DUL, EEPROM_TIMEOUT);

    /* write data from buffer */
    for (uint16_t i = 0; i < len; i++, addr++) {
        iwdg_reload();
        _MEM_(addr) = buf[i];
        EEPROM_WAIT_WHILE(&_if_IAPSR_EOP, EEPROM_TIMEOUT);
    }

    /* lock EEPROM */
    FLASH->IAPSR &= ~(1 << FLASH_IAPSR_DUL);
    
    return TRUE;
}

void eeprom_read(uint16_t addr, uint8_t *buf, int len) {
    /* read EEPROM data into buffer */
    for (int i = 0; i < len; i++, addr++) {
        buf[i] = _MEM_(addr);
    }
}

bool _if_IAPSR_DUL() {
    return FLASH->IAPSR & FLASH_IAPSR_DUL;
}

bool _if_IAPSR_EOP() {
    return FLASH->IAPSR & FLASH_IAPSR_EOP;
}