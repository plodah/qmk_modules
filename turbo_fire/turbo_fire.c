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
    tap_code16( (uint16_t)cb_arg );
    return rate;
}

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record){
    switch(keycode) {

        case COMMUNITY_MODULE_TURBO_A_TOGGLE:
            if(record->event.pressed){
                if(token_a == INVALID_DEFERRED_TOKEN ){
                    token_a = defer_exec(rate, turbo_fire, (void *)keycode_a);
                }
                else{
                    cancel_deferred_exec(token_a);
                    token_a = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_A_MOMENTARY:
            if(record->event.pressed){
                token_a = defer_exec(rate, turbo_fire, (void *)keycode_a);
            }
            else{
                cancel_deferred_exec(token_a);
                token_a = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_B_TOGGLE:
            if(record->event.pressed){
                if(token_b == INVALID_DEFERRED_TOKEN ){
                    token_b = defer_exec(rate, turbo_fire, (void *)keycode_b);
                }
                else{
                    cancel_deferred_exec(token_b);
                    token_b = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_B_MOMENTARY:
            if(record->event.pressed){
                token_b = defer_exec(rate, turbo_fire, (void *)keycode_b);
            }
            else{
                cancel_deferred_exec(token_b);
                token_b = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_C_TOGGLE:
            if(record->event.pressed){
                if(token_c == INVALID_DEFERRED_TOKEN ){
                    token_c = defer_exec(rate, turbo_fire, (void *)keycode_c);
                }
                else{
                    cancel_deferred_exec(token_c);
                    token_c = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_C_MOMENTARY:
            if(record->event.pressed){
                token_c = defer_exec(rate, turbo_fire, (void *)keycode_c);
            }
            else{
                cancel_deferred_exec(token_c);
                token_c = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_D_TOGGLE:
            if(record->event.pressed){
                if(token_d == INVALID_DEFERRED_TOKEN ){
                    token_d = defer_exec(rate, turbo_fire, (void *)keycode_d);
                }
                else{
                    cancel_deferred_exec(token_d);
                    token_d = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_D_MOMENTARY:
            if(record->event.pressed){
                token_d = defer_exec(rate, turbo_fire, (void *)keycode_d);
            }
            else{
                cancel_deferred_exec(token_d);
                token_d = INVALID_DEFERRED_TOKEN;
            }
            return false;
    }
    return true;
}
