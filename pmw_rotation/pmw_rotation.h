#pragma once

#include QMK_KEYBOARD_H
#include "drivers/sensors/pmw33xx_common.h"

#ifndef PMW_ROTATION_STEP_SIZE
    #define PMW_ROTATION_STEP_SIZE 15
#endif
#ifndef PMW_ROTATION_LIMIT
    #define PMW_ROTATION_LIMIT 90
#endif

int8_t pmw_rotation_get_sensor (void);
void pmw_rotation_set_sensor(int8_t);
int8_t pmw_rotation_get_config (void);
void pmw_rotation_set_config (bool absolute, int8_t value);
void pmw_rotation_config_to_sensor (void);
void pmw_rotation_sensor_to_config (void);
bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record);
