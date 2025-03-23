#pragma once
#include "deferred_exec.h"

#ifndef TURBO_FIRE_KEY_A
    #define TURBO_FIRE_KEY_A KC_A
#endif // TURBO_FIRE_KEY_A

#ifndef TURBO_FIRE_KEY_B
    #define TURBO_FIRE_KEY_B KC_B
#endif // TURBO_FIRE_KEY_B

#ifndef TURBO_FIRE_KEY_C
    #define TURBO_FIRE_KEY_C KC_C
#endif // TURBO_FIRE_KEY_C

#ifndef TURBO_FIRE_KEY_D
    #define TURBO_FIRE_KEY_D KC_D
#endif // TURBO_FIRE_KEY_D

#ifndef TURBO_FIRE_RATE
    #define TURBO_FIRE_RATE 25
#endif // TURBO_FIRE_RATE

deferred_token turbo_fire_a_token;
deferred_token turbo_fire_b_token;
deferred_token turbo_fire_c_token;
deferred_token turbo_fire_d_token;

uint8_t turbo_fire_key_a;
uint8_t turbo_fire_key_b;
uint8_t turbo_fire_key_c;
uint8_t turbo_fire_key_d;
uint8_t turbo_fire_rate;
