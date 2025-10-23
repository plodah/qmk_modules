#pragma once

#include QMK_KEYBOARD_H
#ifndef TASKSWITCH_DELAY
    #define TASKSWITCH_DELAY 1000
#endif // TASKSWITCH_DELAY

#if defined(DEFERRED_EXEC_ENABLE) && (!defined(TASKSWITCH_FORCE_NDE))
    #define TASKSWITCH_MODE_DE
    #include "deferred_exec.h"
#endif // DEFERRED_EXEC_ENABLE

#if defined(TASKSWITCH_MODE_DE)
    deferred_token taskSwitchToken;
#else // TASKSWITCH_MODE_DE
    uint16_t taskswitch_timer;
#endif // defined(TASKSWITCH_MODE_DE)
bool is_taskswitch_altregistered;

void taskswitch_press(bool reverse);
void taskswitch_release(void);
