#include <stdbool.h>
#include "quantum.h"
#include "task_switch.h"

#if defined(TASKSWITCH_MODE_DE)
    uint32_t taskswitch_reset(uint32_t trigger_time, void *cb_arg) {
        unregister_code(KC_LALT);
        taskSwitchToken = INVALID_DEFERRED_TOKEN;
        // cancel_deferred_exec(taskSwitchToken);
        return 0;
    }
#else // TASKSWITCH_MODE_DE
    void matrix_scan_task_switch(void) {
        if (is_taskswitch_active) {
            if (timer_elapsed(taskswitch_timer) > TASKSWITCH_DELAY) {
                unregister_code(KC_LALT);
                is_taskswitch_active = false;
            }
        }
    }
#endif // TASKSWITCH_MODE_DE

void taskswitch_start(bool reverse) {
#   if defined(TASKSWITCH_MODE_DE)
    if (taskSwitchToken != INVALID_DEFERRED_TOKEN) {
        extend_deferred_exec(taskSwitchToken, TASKSWITCH_DELAY);
    }
    else{
#   else // TASKSWITCH_MODE_DE
    taskswitch_timer = timer_read();
    if (!is_taskswitch_active) {
#   endif // TASKSWITCH_MODE_DE
        if (!(get_mods() & MOD_MASK_ALT)){
            dprintf("Not a mouse, no alt");
#   if defined(TASKSWITCH_MODE_DE)
            taskSwitchToken = defer_exec(TASKSWITCH_DELAY, taskswitch_reset, NULL);
#   else // TASKSWITCH_MODE_DE
            is_taskswitch_active = true;
#   endif // TASKSWITCH_MODE_DE
            register_code(KC_LALT);
        }
    }
    if(reverse){
        tap_code16(S(KC_TAB));
    }
    else{
        tap_code16(KC_TAB);
    }
}

bool process_record_task_switch(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_task_switch_kb(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case COMMUNITY_MODULE_TASK_SWITCH_NEXT:
            if (record->event.pressed) {
                taskswitch_start(false);
            }
            return false;
        case COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS:
            if (record->event.pressed) {
                taskswitch_start(true);
            }
            return false;
    }
    return true;
}
