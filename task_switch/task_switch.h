#pragma once

#ifndef TASKSWITCH_DELAY
    #define TASKSWITCH_DELAY 1000
#endif // TASKSWITCH_DELAY

#if defined(DEFERRED_EXEC_ENABLE) && (!defined(TASKSWITCH_FORCE_NDE))
    #define TASKSWITCH_MODE_DE
    #include "deferred_exec.h"
#endif // DEFERRED_EXEC_ENABLE

#if defined(TASKSWITCH_MODE_DE)
    static deferred_token taskSwitchToken = INVALID_DEFERRED_TOKEN;
#else // TASKSWITCH_MODE_DE
    bool is_taskswitch_active = false;
    uint16_t taskswitch_timer = 0;
#endif // defined(TASKSWITCH_MODE_DE)

#if defined(TASKSWITCH_MODE_DE)
  uint32_t taskswitch_reset(uint32_t trigger_time, void *cb_arg);
#else // TASKSWITCH_MODE_DE
  void matrix_scan_task_switch(void);
#endif // TASKSWITCH_MODE_DE
void taskswitch_start(bool reverse);
