#include QMK_KEYBOARD_H
#include "repeathold_rgb.h"

uint16_t repeathold_rgb_timer;

bool process_record_repeathold_rgb(uint16_t keycode, keyrecord_t *record) {

    #if defined(RGB_MATRIX_ENABLE)
        switch(keycode){
            case RM_HUEU:
                rm_state.hueu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_HUED:
                rm_state.hued = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_SATU:
                rm_state.satu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_SATD:
                rm_state.satd = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_VALU:
                rm_state.valu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_VALD:
                rm_state.vald = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_SPDU:
                rm_state.spdu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case RM_SPDD:
                rm_state.spdd = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
        }
    #endif // RGB_MATRIX_ENABLE

    #if defined(RGBLIGHT_ENABLE)
        switch(keycode){
            case UG_HUEU:
                ug_state.hueu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_HUED:
                ug_state.hued = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_SATU:
                ug_state.satu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_SATD:
                ug_state.satd = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_VALU:
                ug_state.valu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_VALD:
                ug_state.vald = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_SPDU:
                ug_state.spdu = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
            case UG_SPDD:
                ug_state.spdd = record->event.pressed;
                repeathold_rgb_timer = timer_read();
                return false;
        }
    #endif // defined(RGBLIGHT_ENABLE)
    return true;
}

void housekeeping_task_repeathold_rgb(void){
    if(timer_elapsed(repeathold_rgb_timer) > PLODAH_REPEATHOLD_RATE){
        #if defined(RGB_MATRIX_ENABLE)
            if(rm_state.valu){
                rgb_matrix_increase_val();
            }
            if(rm_state.vald){
                rgb_matrix_decrease_val();
            }
            if(rm_state.satu){
                rgb_matrix_increase_sat();
            }
            if(rm_state.satd){
                rgb_matrix_decrease_sat();
            }
            if(rm_state.hueu){
                rgb_matrix_increase_hue();
            }
            if(rm_state.hued){
                rgb_matrix_decrease_hue();
            }
            if(rm_state.spdu){
                rgb_matrix_increase_speed();
            }
            if(rm_state.spdd){
                rgb_matrix_decrease_speed();
            }
        #endif // defined(RGB_MATRIX_ENABLE)

        #if defined(RGBLIGHT_ENABLE)
            if(ug_state.valu){
                rgblight_increase_val();
            }
            if(ug_state.vald){
                rgblight_decrease_val();
            }
            if(ug_state.satu){
                rgblight_increase_sat();
            }
            if(ug_state.satd){
                rgblight_decrease_sat();
            }
            if(ug_state.hueu){
                rgblight_increase_hue();
            }
            if(ug_state.hued){
                rgblight_decrease_hue();
            }
            if(ug_state.spdu){
                rgblight_increase_speed();
            }
            if(ug_state.spdd){
                rgblight_decrease_speed();
            }
        #endif // defined(RGBLIGHT_ENABLE)

        repeathold_rgb_timer = timer_read();
    }
}
