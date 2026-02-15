#include QMK_KEYBOARD_H
#include "turbo_fire.h"

uint16_t keycode_a = TURBO_FIRE_KEY_A;
uint16_t keycode_b = TURBO_FIRE_KEY_B;
uint16_t keycode_c = TURBO_FIRE_KEY_C;
uint16_t keycode_d = TURBO_FIRE_KEY_D;
uint8_t  rate = TURBO_FIRE_RATE;
deferred_token token_a;
deferred_token token_b;
deferred_token token_c;
deferred_token token_d;

uint32_t turbo_fire(uint32_t trigger_time, void *cb_arg) {
    #if defined(QMK_MCU_RP2040)
        tap_code16( ((uint32_t)cb_arg) & 0xFFFF );
    #else // QMK_MCU_RP2040
        tap_code16( (uint16_t)(cb_arg) );
    #endif // QMK_MCU_RP2040
    return rate;
}

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record){
    switch(keycode) {

        case COMMUNITY_MODULE_TURBO_A_TOGGLE:
            if(record->event.pressed){
                if(token_a == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        token_a = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_a));
                    #else // QMK_MCU_RP2040
                        token_a = defer_exec(rate, turbo_fire, (void *)keycode_a);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(token_a);
                    token_a = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_A_MOMENTARY:
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    token_a = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_a));
                #else // QMK_MCU_RP2040
                    token_a = defer_exec(rate, turbo_fire, (void *)keycode_a);
                #endif // QMK_MCU_RP2040
            }
            else{
                cancel_deferred_exec(token_a);
                token_a = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_B_TOGGLE:
            if(record->event.pressed){
                if(token_b == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        token_b = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_b));
                    #else // QMK_MCU_RP2040
                        token_b = defer_exec(rate, turbo_fire, (void *)keycode_b);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(token_b);
                    token_b = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_B_MOMENTARY:
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    token_b = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_b));
                #else // QMK_MCU_RP2040
                    token_b = defer_exec(rate, turbo_fire, (void *)keycode_b);
                #endif // QMK_MCU_RP2040
            }
            else{
                cancel_deferred_exec(token_b);
                token_b = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_C_TOGGLE:
            if(record->event.pressed){
                if(token_c == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        token_c = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_c));
                    #else // QMK_MCU_RP2040
                        token_c = defer_exec(rate, turbo_fire, (void *)keycode_c);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(token_c);
                    token_c = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_C_MOMENTARY:
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    token_c = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_c));
                #else // QMK_MCU_RP2040
                    token_c = defer_exec(rate, turbo_fire, (void *)keycode_c);
                #endif // QMK_MCU_RP2040
            }
            else{
                cancel_deferred_exec(token_c);
                token_c = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_D_TOGGLE:
            if(record->event.pressed){
                if(token_d == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        token_d = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_d));
                    #else // QMK_MCU_RP2040
                        token_d = defer_exec(rate, turbo_fire, (void *)keycode_d);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(token_d);
                    token_d = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_D_MOMENTARY:
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    token_d = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycode_d));
                #else // QMK_MCU_RP2040
                    token_d = defer_exec(rate, turbo_fire, (void *)keycode_d);
                #endif // QMK_MCU_RP2040
            }
            else{
                cancel_deferred_exec(token_d);
                token_d = INVALID_DEFERRED_TOKEN;
            }
            return false;
    }
    return true;
}
