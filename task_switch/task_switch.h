#pragma once

#include <sys/types.h>
#include QMK_KEYBOARD_H

#ifndef TASKSWITCH_DELAY
    #define TASKSWITCH_DELAY 1000
#endif // TASKSWITCH_DELAY

#ifndef TASKSWITCH_MOD
    #define TASKSWITCH_MOD MOD_LALT
#endif // TASKSWITCH_MOD

#ifndef TASKSWITCH_REVERSE_MOD
    #define TASKSWITCH_REVERSE_MOD MOD_LSFT
#endif // TASKSWITCH_REVERSE_MOD

#ifndef TASKSWITCH_TAP
    #define TASKSWITCH_TAP KC_TAB
#endif // TASKSWITCH_TAP

#if defined(DEFERRED_EXEC_ENABLE) && (!defined(TASKSWITCH_FORCE_NDE))
    #define TASKSWITCH_MODE_DE
    #include "deferred_exec.h"
#endif // DEFERRED_EXEC_ENABLE

void taskswitch_press(bool reverse);
void taskswitch_release(void);

uint8_t taskswitch_get_mod(void);
void taskswitch_set_mod(uint8_t newmod);
uint8_t taskswitch_get_rev_mod(void);
void taskswitch_set_rev_mod(uint8_t newmod);
uint16_t taskswitch_get_tap(void);
void taskswitch_set_tap(uint16_t newtap);
uint16_t taskswitch_get_delay(void);
void taskswitch_set_delay(uint16_t newdelay);
