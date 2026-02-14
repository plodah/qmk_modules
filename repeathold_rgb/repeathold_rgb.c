#include QMK_KEYBOARD_H
#if defined(RGB_MATRIX_ENABLE)
    #include "rgb_matrix.h"
#endif // RGB_MATRIX_ENABLE
#if defined(RGBLIGHT_ENABLE)
    #include "rgblight.h"
#endif // RGBLIGHT_ENABLE

#if defined(RGB_MATRIX_ENABLE)
    int8_t rm_va_state = -1;
    int8_t rm_hu_state = -1;
    int8_t rm_sa_state = -1;
    int8_t rm_sp_state = -1;
#endif // defined(RGB_MATRIX_ENABLE)

#if defined(RGBLIGHT_ENABLE)
    int8_t ug_va_state = -1;
    int8_t ug_hu_state = -1;
    int8_t ug_sa_state = -1;
    int8_t ug_sp_state = -1;
#endif // defined(RGBLIGHT_ENABLE)

uint16_t repeathold_rgb_timer;

bool process_record_repeathold_rgb(uint16_t keycode, keyrecord_t *record) {

    #if defined(RGB_MATRIX_ENABLE)
        switch(keycode){
            case RM_HUEU ... RM_HUED:
                if(record->event.pressed){rm_hu_state = keycode & 0x1;}
                else{rm_hu_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case RM_SATU ... RM_SATD:
                if(record->event.pressed){rm_sa_state = keycode & 0x1;}
                else{rm_sa_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case RM_VALU ... RM_VALD:
                if(record->event.pressed){rm_va_state = keycode & 0x1;}
                else{rm_va_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case RM_SPDU ... RM_SPDD:
                if(record->event.pressed){rm_sp_state = keycode & 0x1;}
                else{rm_sp_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;
        }
    #endif // RGB_MATRIX_ENABLE

    #if defined(RGBLIGHT_ENABLE)
        switch(keycode){
            case UG_HUEU ... UG_HUED:
                if(record->event.pressed){ug_hu_state = keycode & 0x1;}
                else{ug_hu_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case UG_SATU ... UG_SATD:
                if(record->event.pressed){ug_sa_state = keycode & 0x1;}
                else{ug_sa_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case UG_VALU ... UG_VALD:
                if(record->event.pressed){ug_va_state = keycode & 0x1;}
                else{ug_va_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;

            case UG_SPDU ... UG_SPDD:
                if(record->event.pressed){ug_sp_state = keycode & 0x1;}
                else{ug_sp_state = -1;}
                repeathold_rgb_timer = timer_read();
                break;
        }
    #endif // defined(RGBLIGHT_ENABLE)
    return true;
}

void housekeeping_task_repeathold_rgb(void){
    if(timer_elapsed(repeathold_rgb_timer) > PLODAH_REPEATHOLD_RATE){
        #if defined(RGB_MATRIX_ENABLE)
            if(rm_va_state == 1){
                rgb_matrix_increase_val();
            }
            if(rm_va_state == 0){
                rgb_matrix_decrease_val();
            }
            if(rm_sa_state == 1){
                rgb_matrix_increase_sat();
            }
            if(rm_sa_state == 0){
                rgb_matrix_decrease_sat();
            }
            if(rm_hu_state == 1){
                rgb_matrix_increase_hue();
            }
            if(rm_hu_state == 0){
                rgb_matrix_decrease_hue();
            }
            if(rm_sp_state == 1){
                rgb_matrix_increase_speed();
            }
            if(rm_sp_state == 0){
                rgb_matrix_decrease_speed();
            }
        #endif // defined(RGB_MATRIX_ENABLE)

        #if defined(RGBLIGHT_ENABLE)
            if(ug_va_state == 1){
                rgblight_increase_val();
            }
            if(ug_va_state == 0){
                rgblight_decrease_val();
            }
            if(ug_sa_state == 1){
                rgblight_increase_sat();
            }
            if(ug_sa_state == 0){
                rgblight_decrease_sat();
            }
            if(ug_hu_state == 1){
                rgblight_increase_hue();
            }
            if(ug_hu_state == 0){
                rgblight_decrease_hue();
            }
            if(ug_sp_state == 1){
                rgblight_increase_speed();
            }
            if(ug_sp_state == 0){
                rgblight_decrease_speed();
            }
        #endif // defined(RGBLIGHT_ENABLE)

        repeathold_rgb_timer = timer_read();
    }
}
