#include QMK_KEYBOARD_H
#include "rgblight_timeout.h"

void housekeeping_task_rgblight_timeout ( void ) {
    if ( last_input_activity_elapsed() > RGBLIGHT_TIMEOUT ) {
        if( ! rgblight_disabled_by_timeout ){
            rgblight_disabled_by_timeout = true;
            rgblight_disable_noeeprom();
        }
    }
    else {
        if( rgblight_disabled_by_timeout ){
            rgblight_disabled_by_timeout = false;
            rgblight_enable_noeeprom();
        }
    }
}
