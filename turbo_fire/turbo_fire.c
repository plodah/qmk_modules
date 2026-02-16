#include QMK_KEYBOARD_H
#include "turbo_fire.h"

uint16_t keycodes[TURBO_FIRE_KEYCOUNT];
deferred_token tokens[TURBO_FIRE_KEYCOUNT];
uint16_t rate = TURBO_FIRE_RATE;
uint8_t duration = TURBO_FIRE_DURATION;

uint8_t get_turbo_fire_keycount ( void ){
    return TURBO_FIRE_KEYCOUNT;
}

void set_turbo_fire_keycode ( uint8_t index, int16_t keycode ){
    keycodes[index] = keycode;
}

uint16_t get_turbo_fire_keycode ( uint8_t index ){
    return keycodes[index];
}

void set_turbo_fire_rate ( uint16_t newrate ){
    rate = newrate;
}

uint16_t get_turbo_fire_rate ( void ){
    return rate;
}

void set_turbo_fire_duration ( uint8_t newduration ){
    duration = newduration;
}

uint8_t get_turbo_fire_duration ( void ){
    return duration;
}

void keyboard_post_init_turbo_fire (void) {
    for (uint8_t i = 0; i < TURBO_FIRE_KEYCOUNT; i++) {
        keycodes[i] = default_turbo_fire_keycodes[i];
    }
}

uint32_t turbo_fire(uint32_t trigger_time, void *cb_arg) {
    #if defined(QMK_MCU_RP2040)
        tap_code16_delay( ((uint32_t)cb_arg) & 0xFFFF, duration );
    #else // QMK_MCU_RP2040
        tap_code16_delay( (uint16_t)(cb_arg), duration );
    #endif // QMK_MCU_RP2040
    return rate;
}

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record){
    uint16_t kc_index;
    switch(keycode) {
        case COMMUNITY_MODULE_TURBO_A_TOGGLE:
        #if TURBO_FIRE_KEYCOUNT > 1
            case COMMUNITY_MODULE_TURBO_B_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 2
            case COMMUNITY_MODULE_TURBO_C_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 3
            case COMMUNITY_MODULE_TURBO_D_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 4
            case COMMUNITY_MODULE_TURBO_E_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 5
            case COMMUNITY_MODULE_TURBO_F_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 6
            case COMMUNITY_MODULE_TURBO_G_TOGGLE:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 7
            case COMMUNITY_MODULE_TURBO_H_TOGGLE:
        #endif
            kc_index = keycode - COMMUNITY_MODULE_TURBO_A_TOGGLE;
            dprintf("kc:%d index: %d\n", keycode, kc_index);
            if(record->event.pressed){
                if(tokens[kc_index] == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        tokens[kc_index] = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycodes[kc_index]));
                    #else // QMK_MCU_RP2040
                        tokens[kc_index] = defer_exec(rate, turbo_fire, (void *)keycodes[kc_index]);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(tokens[kc_index]);
                    tokens[kc_index] = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_A_MOMENTARY:
        #if TURBO_FIRE_KEYCOUNT > 1
            case COMMUNITY_MODULE_TURBO_B_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 2
            case COMMUNITY_MODULE_TURBO_C_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 3
            case COMMUNITY_MODULE_TURBO_D_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 4
            case COMMUNITY_MODULE_TURBO_E_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 5
            case COMMUNITY_MODULE_TURBO_F_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 6
            case COMMUNITY_MODULE_TURBO_G_MOMENTARY:
        #endif
        #if TURBO_FIRE_KEYCOUNT > 7
            case COMMUNITY_MODULE_TURBO_H_MOMENTARY:
        #endif
            kc_index = keycode - COMMUNITY_MODULE_TURBO_A_TOGGLE;
            dprintf("kc:%d index: %d\n", keycode, kc_index);
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    tokens[kc_index] = defer_exec(rate, turbo_fire, (void *)((uint32_t)keycodes[kc_index]));
                #else // QMK_MCU_RP2040
                    tokens[kc_index] = defer_exec(rate, turbo_fire, (void *)keycodes[kc_index]);
                #endif // QMK_MCU_RP2040
            }
            else{
                cancel_deferred_exec(tokens[kc_index]);
                tokens[kc_index] = INVALID_DEFERRED_TOKEN;
            }
            return false;
    }
    return true;
}
