#pragma once
#include QMK_KEYBOARD_H

#if ! defined(TURBO_FIRE_KEYCOUNT)
    #define TURBO_FIRE_KEYCOUNT 4
#endif // TURBO_FIRE_KEYCOUNT

#if TURBO_FIRE_KEYCOUNT > 8
    #undef TURBO_FIRE_KEYCOUNT
    #define TURBO_FIRE_KEYCOUNT 8
    #pragma message "TURBO_FIRE_KEYCOUNT can't be more than 8. Set to 8."
#endif

#ifndef TURBO_FIRE_RATE
    #define TURBO_FIRE_RATE 50
#endif // TURBO_FIRE_RATE

#ifndef TURBO_FIRE_DURATION
    #define TURBO_FIRE_DURATION 10
#endif // TURBO_FIRE_DURATION

static const uint16_t default_turbo_fire_keycodes[]={
    MS_BTN1,
    #if TURBO_FIRE_KEYCOUNT > 1
        MS_BTN2,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 2
        KC_A,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 3
        KC_B,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 4
        KC_C,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 5
        KC_D,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 6
        KC_E,
    #endif
    #if TURBO_FIRE_KEYCOUNT > 7
        KC_F,
    #endif
};

uint8_t get_turbo_fire_keycount ( void );
void set_turbo_fire_keycode ( uint8_t index, int16_t keycode );
uint16_t get_turbo_fire_keycode ( uint8_t index );
void set_turbo_fire_rate ( uint16_t newrate );
uint16_t get_turbo_fire_rate ( void );
void set_turbo_fire_duration ( uint8_t newduration );
uint8_t get_turbo_fire_duration ( void );
void keyboard_post_init_turbo_fire ( void );
