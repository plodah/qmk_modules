#include <stdbool.h>
#include "quantum.h"
#include "task_switch.h"

#if defined(TASKSWITCH_MODE_DE)
    uint32_t taskswitch_reset(uint32_t trigger_time, void *cb_arg) {
        unregister_code(KC_LALT);
        is_taskswitch_altregistered = false;
        taskSwitchToken = INVALID_DEFERRED_TOKEN;
        // cancel_deferred_exec(taskSwitchToken);
        return 0;
    }
#else // TASKSWITCH_MODE_DE
    void housekeeping_task_task_switch(void) {
        if (is_taskswitch_altregistered) {
            if (timer_elapsed(taskswitch_timer) > TASKSWITCH_DELAY) {
                unregister_code(KC_LALT);
                is_taskswitch_altregistered = false;
            }
        }
    }
#endif // TASKSWITCH_MODE_DE

void taskswitch_press(bool reverse) {
    #if defined(TASKSWITCH_MODE_DE)
        if (taskSwitchToken != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec(taskSwitchToken);
        }
    #endif // TASKSWITCH_MODE_DE

    if (!(get_mods() & MOD_MASK_ALT)){
        is_taskswitch_altregistered = true;
        register_code(KC_LALT);
    }
    
    if(reverse){
        tap_code16(S(KC_TAB));
    }
    else{
        tap_code16(KC_TAB);
    }
}

void taskswitch_release(void) {
    if(is_taskswitch_altregistered){
        #if defined(TASKSWITCH_MODE_DE)
            taskSwitchToken = defer_exec(TASKSWITCH_DELAY, taskswitch_reset, NULL);
        #else // TASKSWITCH_MODE_DE
            taskswitch_timer = timer_read();
        #endif // TASKSWITCH_MODE_DE
    }
}

bool process_record_task_switch(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_task_switch_kb(keycode, record)) {
        return false;
    }
    if (record->event.pressed) {
        switch (keycode) {
            case COMMUNITY_MODULE_TASK_SWITCH_NEXT:
                taskswitch_press(false);
                return false;
            case COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS:
                taskswitch_press(true);
                return false;
        }
    }
    else{
        switch (keycode) {
            case COMMUNITY_MODULE_TASK_SWITCH_NEXT:
            case COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS:
                taskswitch_release();
                return false;
        }

    }
    return true;
}
