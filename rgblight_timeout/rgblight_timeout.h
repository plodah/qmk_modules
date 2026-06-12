#pragma once
#include "rgblight.h"

bool rgblight_disabled_by_timeout;
void housekeeping_task_rgblight_timeout ( void );

#ifndef RGBLIGHT_TIMEOUT
    #define RGBLIGHT_TIMEOUT ( 15 * 60 * 1000 ) // 15 minutes
#endif
