#pragma once

#include QMK_KEYBOARD_H
#include "drivers/sensors/pmw33xx_common.h"

#ifdef ROTATIONAL_TRANSFORM_ANGLE
    int16_t pmwrotation = ROTATIONAL_TRANSFORM_ANGLE;
#else
    int16_t pmwrotation = 0;
#endif // ROTATIONAL_TRANSFORM_ANGLE

#ifndef PMW_ROTATION_STEP_SIZE
    #define PMW_ROTATION_STEP_SIZE 16
#endif

void pmw_rotation_act (bool absolute, int16_t value);
bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record);
