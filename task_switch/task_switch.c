#include QMK_KEYBOARD_H
#include "task_switch.h"

#if defined(TASKSWITCH_MODE_DE)
    deferred_token taskSwitchToken = INVALID_DEFERRED_TOKEN;
#else // TASKSWITCH_MODE_DE
    uint16_t taskswitch_timer = 0;
#endif // defined(TASKSWITCH_MODE_DE)

bool mod_registered;
uint8_t mod = TASKSWITCH_MOD;
uint8_t rev_mod = TASKSWITCH_REVERSE_MOD;
uint16_t tap = (TASKSWITCH_TAP & 0xFF);
uint16_t delay = TASKSWITCH_DELAY;

void taskswitch_reset(void){
    mod_registered = false;
    unregister_mods(mod);
}

#if defined(TASKSWITCH_MODE_DE)
    uint32_t taskswitch_deferred_task(uint32_t trigger_time, void *cb_arg) {
        taskswitch_reset();
        taskSwitchToken = INVALID_DEFERRED_TOKEN;
        dprintf("taskswitch_deferred_task\n");
        return 0;
    }
#else // TASKSWITCH_MODE_DE
    void housekeeping_task_task_switch(void) {
        if (mod_registered) {
            if (timer_elapsed(taskswitch_timer) > delay) {
                taskswitch_reset();
                dprintf("taskswitch timer elapsed\n");
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
    if (!(get_mods() & mod)){
        dprintf("taskswitch sends mod: %d\n", mod);
        mod_registered = true;
        register_mods(mod);
    }
    uint16_t sendkc = tap;
    if(reverse){
        // Remove the standard `mod` from the `rev_mod`
        // shift calculated mod left 8 bits to create  e.g. `S(KC_NO)`
        // then add `tap`
        sendkc = (((rev_mod ^ mod) & rev_mod) << 8) | tap;
    }
    dprintf("taskswitch press %d\n", sendkc);
    tap_code16(sendkc);
}

void taskswitch_release(void) {
    dprintf("taskswitch release\n");
    if(mod_registered){
        #if defined(TASKSWITCH_MODE_DE)
            taskSwitchToken = defer_exec(delay, taskswitch_deferred_task, NULL);
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

uint8_t taskswitch_get_mod(void){
    return mod;
}
void taskswitch_set_mod(uint8_t newmod){
    dprintf("set_mod: %d\n", newmod);
    mod = newmod;
}

uint8_t taskswitch_get_rev_mod(void){
    return rev_mod;
}
void taskswitch_set_rev_mod(uint8_t newmod){
    dprintf("set_rev_mod: %d\n", newmod);
    rev_mod = newmod;
}

uint16_t taskswitch_get_tap(void){
    return tap;
}
void taskswitch_set_tap(uint16_t newtap){
    dprintf("set_tap: %d -> %d\n", newtap, newtap & 0xff);
    tap = newtap & 0xff;
}

uint16_t taskswitch_get_delay(void){
    return delay;
}
void taskswitch_set_delay(uint16_t newdelay){
    dprintf("set_delay: %d\n", newdelay);
    delay = newdelay;
}
