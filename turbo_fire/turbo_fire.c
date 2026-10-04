#include QMK_KEYBOARD_H
#include "turbo_fire.h"
#include "eeconfig.h"

deferred_token tokens[TURBO_FIRE_KEYCOUNT];

typedef struct turbo_fire_config_t {
    uint16_t    rate            :16;
    uint8_t     duration        :8;
    uint16_t    keycodes[TURBO_FIRE_KEYCOUNT];
} PACKED turbo_fire_config_t;
turbo_fire_config_t turbo_fire_config;

const turbo_fire_config_t PROGMEM turbo_fire_default_config = {
    .rate = TURBO_FIRE_RATE,
    .duration = TURBO_FIRE_DURATION,
    .keycodes = {
        MS_BTN1,
        #if TURBO_FIRE_KEYCOUNT >= 2
            MS_BTN2,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 3
            KC_A,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 4
            KC_B,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 5
            KC_C,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 6
            KC_D,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 7
            KC_E,
        #endif
        #if TURBO_FIRE_KEYCOUNT >= 8
            KC_F,
        #endif
    },
};

_Static_assert(sizeof(turbo_fire_config_t) <= EECONFIG_MODULE_TURBO_FIRE_DATA_SIZE, "EECONFIG_MODULE_TURBO_FIRE_DATA_SIZE is too small");

void eeconfig_read_turbo_fire(turbo_fire_config_t *value) {
    eeconfig_read_turbo_fire_datablock(value, 0, sizeof(turbo_fire_config_t));
}

void eeconfig_update_turbo_fire(turbo_fire_config_t *value) {
    eeconfig_update_turbo_fire_datablock(value, 0, sizeof(turbo_fire_config_t));
}

EECONFIG_DEBOUNCE_HELPER(turbo_fire, turbo_fire_config);

void keyboard_post_init_turbo_fire(void) {
    eeconfig_init_turbo_fire();
}

void eeconfig_init_turbo_fire_datablock(void) {
    turbo_fire_config = turbo_fire_default_config;
    eeconfig_flush_turbo_fire(true);
}

void housekeeping_task_turbo_fire(void) {
    eeconfig_flush_turbo_fire_task(1000);
}

uint8_t get_turbo_fire_keycount ( void ){
    return TURBO_FIRE_KEYCOUNT;
}

void set_turbo_fire_keycode ( uint8_t index, int16_t keycode ){
    dprintf("set_turbo_fire_keycode i:%d kc:%d\n", index, keycode);
    turbo_fire_config.keycodes[index] = keycode;
    eeconfig_flag_turbo_fire(true);
}

uint16_t get_turbo_fire_keycode ( uint8_t index ){
    return turbo_fire_config.keycodes[index];
}

void set_turbo_fire_rate ( uint16_t newrate ){
    dprintf("set_turbo_fire_rate %d\n", newrate);
    turbo_fire_config.rate = newrate;
    eeconfig_flag_turbo_fire(true);
}

uint16_t get_turbo_fire_rate ( void ){
    return turbo_fire_config.rate;
}

void set_turbo_fire_duration ( uint8_t newduration ){
    dprintf("set_turbo_fire_duration %d\n", newduration);
    turbo_fire_config.duration = newduration;
    eeconfig_flag_turbo_fire(true);
}

uint8_t get_turbo_fire_duration ( void ){
    return turbo_fire_config.duration;
}

uint32_t turbo_fire_runner(uint32_t trigger_time, void *cb_arg) {
    uint16_t tmpkc;
    #if defined(QMK_MCU_RP2040)
        tmpkc = ((uint32_t)cb_arg) & 0xFFFF;
    #else // QMK_MCU_RP2040
        tmpkc = (uint16_t)(cb_arg);
    #endif // QMK_MCU_RP2040
    dprintf("FIRE: kc:%d, duration:%d rate: %d\n", tmpkc, turbo_fire_config.duration, turbo_fire_config.rate);
    tap_code16_delay( tmpkc, turbo_fire_config.duration );
    return turbo_fire_config.rate;
}

bool process_record_turbo_fire(uint16_t keycode, keyrecord_t *record){
    uint16_t kc_index;
    switch(keycode) {
        case COMMUNITY_MODULE_TURBO_A_TOGGLE:
        case COMMUNITY_MODULE_TURBO_B_TOGGLE:
        case COMMUNITY_MODULE_TURBO_C_TOGGLE:
        case COMMUNITY_MODULE_TURBO_D_TOGGLE:
        case COMMUNITY_MODULE_TURBO_E_TOGGLE:
        case COMMUNITY_MODULE_TURBO_F_TOGGLE:
        case COMMUNITY_MODULE_TURBO_G_TOGGLE:
        case COMMUNITY_MODULE_TURBO_H_TOGGLE:
            kc_index = (keycode - COMMUNITY_MODULE_TURBO_A_TOGGLE)/2;
            dprintf("kc:%d index: %d\n", keycode, kc_index);
            if(record->event.pressed){
                if(tokens[kc_index] == INVALID_DEFERRED_TOKEN ){
                    #if defined(QMK_MCU_RP2040)
                        tokens[kc_index] = defer_exec(turbo_fire_config.rate, turbo_fire_runner, (void *)((uint32_t)turbo_fire_config.keycodes[kc_index]));
                    #else // QMK_MCU_RP2040
                        tokens[kc_index] = defer_exec(turbo_fire_config.rate, turbo_fire_runner, (void *)turbo_fire_config.keycodes[kc_index]);
                    #endif // QMK_MCU_RP2040
                }
                else{
                    cancel_deferred_exec(tokens[kc_index]);
                    tokens[kc_index] = INVALID_DEFERRED_TOKEN;
                }
            }
            return false;
        case COMMUNITY_MODULE_TURBO_A_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_B_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_C_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_D_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_E_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_F_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_G_MOMENTARY:
        case COMMUNITY_MODULE_TURBO_H_MOMENTARY:
            kc_index = (keycode - COMMUNITY_MODULE_TURBO_A_MOMENTARY)/2;
            dprintf("kc:%d index: %d\n", keycode, kc_index);
            if(record->event.pressed){
                #if defined(QMK_MCU_RP2040)
                    tokens[kc_index] = defer_exec(turbo_fire_config.rate, turbo_fire_runner, (void *)((uint32_t)turbo_fire_config.keycodes[kc_index]));
                #else // QMK_MCU_RP2040
                    tokens[kc_index] = defer_exec(turbo_fire_config.rate, turbo_fire_runner, (void *)turbo_fire_config.keycodes[kc_index]);
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
