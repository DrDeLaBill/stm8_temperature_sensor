#ifndef _SETTINGS_H_
#define _SETTINGS_H_


#include <stdint.h>

#include "stm8s.h"


#define CHAR_PARAM_LEN 16


typedef struct _sensor_settings {
    char name[CHAR_PARAM_LEN];
    uint8_t mb_id;
    uint8_t crc;
} sensor_settings;


extern sensor_settings sttngs;


void sttngs_reset();
bool sttngs_load();
void sttngs_save();

void update_mb_id(uint8_t new_id);


#endif