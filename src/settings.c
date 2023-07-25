#include "settings.h"

#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "utils.h"
#include "eeprom.h"


sensor_settings sttngs;
const char* device_name = "TEMPSENSV01";


void _update_crc();


void sttngs_reset() {
    memset((uint8_t*)&sttngs, 0, sizeof(sttngs));
    memcpy(sttngs.name, device_name, sizeof(sttngs.name));
    sttngs.mb_id = DEFAULT_DEVICE_ID;
    _update_crc();
    sttngs_save();
}

bool sttngs_load() {
    sensor_settings tmp_buf;
    memset((uint8_t*)&tmp_buf, 0, sizeof(tmp_buf));

    eeprom_read(EEPROM_START_ADDR, (uint8_t*)&tmp_buf, sizeof(tmp_buf));

    if (memcmp(tmp_buf.name, device_name, sizeof(tmp_buf.name))) {
        return false;
    }

    if (tmp_buf.mb_id > MAX_SLAVE_ID || tmp_buf.mb_id == 0) {
        return false;
    }

    uint8_t tmp_crc = get_crc8((uint8_t*)&tmp_buf, sizeof(tmp_buf) - 1);
    if (tmp_crc != tmp_buf.crc) {
        return false;
    }

    memcpy((uint8_t*)&sttngs, (uint8_t*)&tmp_buf, sizeof(sttngs));

    return true;
}

void sttngs_save() {
    _update_crc();
    eeprom_write(EEPROM_START_ADDR, (uint8_t*)&sttngs, sizeof(sttngs));
}

void sttngs_update_mb_id(uint8_t new_id) {
    if (new_id > MAX_SLAVE_ID || new_id == 0) {
        return;
    }
    sttngs.mb_id = new_id;
    sttngs_save();
}

void _update_crc() {
    sttngs.crc = get_crc8((uint8_t*)&sttngs, sizeof(sttngs) - 1);
}