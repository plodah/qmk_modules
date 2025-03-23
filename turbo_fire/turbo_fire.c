#include "turbo_fire.h"
#include "quantum.h"

uint8_t turbo_fire_key_a = TURBO_FIRE_KEY_A;
uint8_t turbo_fire_key_b = TURBO_FIRE_KEY_B;
uint8_t turbo_fire_key_c = TURBO_FIRE_KEY_C;
uint8_t turbo_fire_key_d = TURBO_FIRE_KEY_D;
uint8_t turbo_fire_rate = TURBO_FIRE_RATE;

uint32_t turbo_fire_a(uint32_t trigger_time, void *cb_arg) {
    tap_code16( turbo_fire_key_a );
    return turbo_fire_rate;
}
uint32_t turbo_fire_b(uint32_t trigger_time, void *cb_arg) {
    tap_code16( turbo_fire_key_b );
    return turbo_fire_rate;
}
uint32_t turbo_fire_c(uint32_t trigger_time, void *cb_arg) {
    tap_code16( turbo_fire_key_c );
    return turbo_fire_rate;
}
uint32_t turbo_fire_d(uint32_t trigger_time, void *cb_arg) {
    tap_code16( turbo_fire_key_d );
    return turbo_fire_rate;
}

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record){
    switch(keycode) {

        case COMMUNITY_MODULE_TURBO_A_TOGGLE:
            if(record->event.pressed){
                if(turbo_fire_a_token == INVALID_DEFERRED_TOKEN ){
                    turbo_fire_a_token = defer_exec(turbo_fire_rate, turbo_fire_a, NULL);
                }
                else{
                    cancel_deferred_exec(turbo_fire_a_token);
                    turbo_fire_a_token = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_A_MOMENTARY:
            if(record->event.pressed){
                turbo_fire_a_token = defer_exec(turbo_fire_rate, turbo_fire_a, NULL);
            }
            else{
                cancel_deferred_exec(turbo_fire_a_token);
                turbo_fire_a_token = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_B_TOGGLE:
            if(record->event.pressed){
                if(turbo_fire_b_token == INVALID_DEFERRED_TOKEN ){
                    turbo_fire_b_token = defer_exec(turbo_fire_rate, turbo_fire_b, NULL);
                }
                else{
                    cancel_deferred_exec(turbo_fire_b_token);
                    turbo_fire_b_token = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_B_MOMENTARY:
            if(record->event.pressed){
                turbo_fire_b_token = defer_exec(turbo_fire_rate, turbo_fire_b, NULL);
            }
            else{
                cancel_deferred_exec(turbo_fire_b_token);
                turbo_fire_b_token = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_C_TOGGLE:
            if(record->event.pressed){
                if(turbo_fire_c_token == INVALID_DEFERRED_TOKEN ){
                    turbo_fire_c_token = defer_exec(turbo_fire_rate, turbo_fire_c, NULL);
                }
                else{
                    cancel_deferred_exec(turbo_fire_c_token);
                    turbo_fire_c_token = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_C_MOMENTARY:
            if(record->event.pressed){
                turbo_fire_c_token = defer_exec(turbo_fire_rate, turbo_fire_c, NULL);
            }
            else{
                cancel_deferred_exec(turbo_fire_c_token);
                turbo_fire_c_token = INVALID_DEFERRED_TOKEN;
            }
            return false;

        case COMMUNITY_MODULE_TURBO_D_TOGGLE:
            if(record->event.pressed){
                if(turbo_fire_d_token == INVALID_DEFERRED_TOKEN ){
                    turbo_fire_d_token = defer_exec(turbo_fire_rate, turbo_fire_d, NULL);
                }
                else{
                    cancel_deferred_exec(turbo_fire_d_token);
                    turbo_fire_d_token = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_D_MOMENTARY:
            if(record->event.pressed){
                turbo_fire_d_token = defer_exec(turbo_fire_rate, turbo_fire_d, NULL);
            }
            else{
                cancel_deferred_exec(turbo_fire_d_token);
                turbo_fire_d_token = INVALID_DEFERRED_TOKEN;
            }
            return false;
    }
    return true;
}
