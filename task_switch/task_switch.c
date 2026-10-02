#include QMK_KEYBOARD_H
#include "task_switch.h"
#include "os_detection.h"

#if defined(DEFERRED_EXEC_ENABLE) && (!defined(TASK_SWITCH_FORCE_NDE))
    #define TASK_SWITCH_MODE_DE
    #include "deferred_exec.h"
#endif // DEFERRED_EXEC_ENABLE

#ifndef TASK_SWITCH_DELAY
    #define TASK_SWITCH_DELAY 1000
#endif // TASK_SWITCH_DELAY

#ifndef TASK_SWITCH_MOD
    #define TASK_SWITCH_MOD MOD_LALT
#endif // TASK_SWITCH_MOD

#ifndef TASK_SWITCH_REVERSE_MOD
    #define TASK_SWITCH_REVERSE_MOD MOD_LSFT
#endif // TASK_SWITCH_REVERSE_MOD

#ifndef TASK_SWITCH_TAP
    #define TASK_SWITCH_TAP KC_TAB
#endif // TASK_SWITCH_TAP

#ifndef TASK_SWITCH_TAP_R
    #define TASK_SWITCH_TAP_R KC_TAB
#endif // TASK_SWITCH_TAP_R

#ifndef TASK_SWITCH_MODE_WINDOWS
    #define TASK_SWITCH_MODE_WINDOWS TASK_SWITCH_CONFIG_SET_WINDOWS
#endif // TASK_SWITCH_MODE_WINDOWS

#ifndef TASK_SWITCH_MODE_MACOS
    #define TASK_SWITCH_MODE_MACOS TASK_SWITCH_CONFIG_SET_MACOS
#endif // TASK_SWITCH_MODE_MACOS

#ifndef TASK_SWITCH_MODE_LINUX
    #define TASK_SWITCH_MODE_LINUX TASK_SWITCH_CONFIG_SET_WINDOWS
#endif // TASK_SWITCH_MODE_LINUX

#ifndef TASK_SWITCH_MODE_DEFAULT
    #define TASK_SWITCH_MODE_DEFAULT TASK_SWITCH_CONFIG_SET_WINDOWS
#endif // TASK_SWITCH_MODE_DEFAULT

typedef struct task_switch_osconf_t{
    uint8_t     mod     :5;
    uint8_t     mod_r   :5;
    uint16_t    tap;
    uint16_t    tap_r;
} PACKED task_switch_osconf_t;

static const task_switch_osconf_t task_switch_osconf_win = {
    .mod    = MOD_LALT,
    .mod_r  = MOD_LSFT,
    .tap    = KC_TAB,
    .tap_r  = KC_TAB,
};

static const task_switch_osconf_t task_switch_osconf_mac = {
    .mod    = MOD_LGUI,
    .mod_r  = MOD_LSFT,
    .tap    = KC_TAB,
    .tap_r  = KC_TAB,
};

static const task_switch_osconf_t task_switch_osconf_default = {
    .mod    = TASK_SWITCH_MOD,
    .mod_r  = TASK_SWITCH_REVERSE_MOD,
    .tap    = TASK_SWITCH_TAP,
    .tap_r  = TASK_SWITCH_TAP_R,
};

#if defined(TASK_SWITCH_NOEEPROM)
    uint16_t delay = TASK_SWITCH_DELAY;
#else
    #include "eeconfig.h"
    typedef struct task_switch_config_t  {
        task_switch_osconf_t    customosconf_a; // 42b (48?) => 6B
        task_switch_osconf_t    customosconf_b; // 42b (48?) => 6B
        uint8_t                 windows_conf        :4;
        uint8_t                 macos_conf          :4;
        uint8_t                 linux_conf          :4;
        uint8_t                 unknownos_conf      :4;
        uint8_t                 manual_conf         :4;
        bool                    use_os_detection    :1; // sum 21b => 3B
        uint16_t                delay;              // 16 => 2B
    } PACKED task_switch_config_t;
    task_switch_config_t task_switch_config;

    task_switch_config_t task_switch_default_config = {
        .customosconf_a     = task_switch_osconf_default,
        .customosconf_b     = task_switch_osconf_default,
        .windows_conf       = TASK_SWITCH_MODE_WINDOWS,
        .macos_conf         = TASK_SWITCH_MODE_MACOS,
        .linux_conf         = TASK_SWITCH_MODE_LINUX,
        .unknownos_conf     = TASK_SWITCH_MODE_DEFAULT,
        .manual_conf        = TASK_SWITCH_MODE_DEFAULT,
        #ifdef TASK_SWITCH_DISABLE_OS_DETECTION
            .use_os_detection   = false,
        #else
            .use_os_detection   = true,
        #endif
        .delay  = TASK_SWITCH_DELAY,
    };

    _Static_assert(sizeof(task_switch_config_t) <= EECONFIG_MODULE_TASK_SWITCH_DATA_SIZE, "EECONFIG_MODULE_TASK_SWITCH_DATA_SIZE is too small");

    void eeconfig_read_task_switch(task_switch_config_t *value) {
        eeconfig_read_task_switch_datablock(value, 0, sizeof(task_switch_config_t));
    }

    void eeconfig_update_task_switch(task_switch_config_t *value) {
        eeconfig_update_task_switch_datablock(value, 0, sizeof(task_switch_config_t));
    }

    EECONFIG_DEBOUNCE_HELPER(task_switch, task_switch_config);

    void keyboard_post_init_task_switch(void) {
        eeconfig_init_task_switch();
    }

    void eeconfig_init_task_switch_datablock(void) {
        task_switch_config = task_switch_default_config;
        eeconfig_flush_task_switch(true);
    }

    void housekeeping_task_task_switch(void) {
        eeconfig_flush_task_switch_task(1000);
    }

    task_switch_osconf_t task_switch_get_config_object(uint8_t id) {
        switch (id) {
            case TASK_SWITCH_CONFIG_SET_WINDOWS:
                return task_switch_osconf_win;
            case TASK_SWITCH_CONFIG_SET_MACOS:
                return task_switch_osconf_mac;
        #if !defined(TASK_SWITCH_NOEEPROM)
            case TASK_SWITCH_CONFIG_SET_CUSTOM_A:
                return task_switch_config.customosconf_a;
            case TASK_SWITCH_CONFIG_SET_CUSTOM_B:
                return task_switch_config.customosconf_b;
        #endif
        }
        return task_switch_osconf_default;
    }

    void task_switch_set_config_object(uint8_t id, task_switch_osconf_t newobject){
        #if !defined(TASK_SWITCH_NOEEPROM)
            switch (id) {
                case TASK_SWITCH_CONFIG_SET_CUSTOM_A:
                    task_switch_config.customosconf_a = newobject;
                    break;
                case TASK_SWITCH_CONFIG_SET_CUSTOM_B:
                    task_switch_config.customosconf_b = newobject;
                    break;
            }
        #endif
    }

    void task_switch_set_eeconfig_key(bool configb, int8_t key, uint16_t newvalue){
        task_switch_osconf_t actobj = task_switch_get_config_object(configb ? TASK_SWITCH_CONFIG_SET_CUSTOM_B : TASK_SWITCH_CONFIG_SET_CUSTOM_A);
        switch(key){
            case TASK_SWITCH_CONFIG_KEY_MOD:
                actobj.mod = newvalue & 0x1F;
                break;
            case TASK_SWITCH_CONFIG_KEY_RMOD:
                actobj.mod_r = newvalue & 0x1F;
                break;
            case TASK_SWITCH_CONFIG_KEY_TAP:
                actobj.tap = newvalue & 0xFF;
                break;
            case TASK_SWITCH_CONFIG_KEY_RTAP:
                actobj.tap_r = newvalue & 0xFF;
                break;
        }
        task_switch_set_config_object((configb ? TASK_SWITCH_CONFIG_SET_CUSTOM_B : TASK_SWITCH_CONFIG_SET_CUSTOM_A), actobj);
        eeconfig_flag_task_switch(true);
    }

    uint16_t task_switch_get_eeconfig_key(bool configb, int8_t key){
        task_switch_osconf_t actobj = task_switch_get_config_object(configb ? TASK_SWITCH_CONFIG_SET_CUSTOM_B : TASK_SWITCH_CONFIG_SET_CUSTOM_A);
        switch(key){
            case TASK_SWITCH_CONFIG_KEY_MOD:
                return actobj.mod;
                break;
            case TASK_SWITCH_CONFIG_KEY_RMOD:
                return actobj.mod_r;
                break;
            case TASK_SWITCH_CONFIG_KEY_TAP:
                return actobj.tap;
                break;
            case TASK_SWITCH_CONFIG_KEY_RTAP:
                return actobj.tap_r;
                break;
        }
        return 0;
    }

    uint8_t task_switch_get_eeconfig_configset(uint8_t configset){
        switch(configset){
            case TASK_SWITCH_OPTION_WINDOWS:
                return task_switch_config.windows_conf;
            case TASK_SWITCH_OPTION_MACOS:
                return task_switch_config.macos_conf;
            case TASK_SWITCH_OPTION_LINUX:
                return task_switch_config.linux_conf;
            case TASK_SWITCH_OPTION_UNKNOWNOS:
                return task_switch_config.unknownos_conf;
            case TASK_SWITCH_OPTION_MANUAL:
                return task_switch_config.manual_conf;
        }
        return TASK_SWITCH_CONFIG_SET_INVALID;
    }

    void task_switch_set_eeconfig_configset(uint8_t configset, uint8_t newoption){
        switch(configset){
            case TASK_SWITCH_OPTION_WINDOWS:
                task_switch_config.windows_conf = newoption;
            case TASK_SWITCH_OPTION_MACOS:
                task_switch_config.macos_conf = newoption;
            case TASK_SWITCH_OPTION_LINUX:
                task_switch_config.linux_conf = newoption;
            case TASK_SWITCH_OPTION_UNKNOWNOS:
                task_switch_config.unknownos_conf = newoption;
            case TASK_SWITCH_OPTION_MANUAL:
                task_switch_config.manual_conf = newoption;
        }
    }

    bool task_switch_get_os_detection_state(void){
        return task_switch_config.use_os_detection;
    }

    void task_switch_set_os_detection_state(bool newstate){
        task_switch_config.use_os_detection = newstate;
    }
#endif // TASK_SWITCH_NOEEPROM

#if defined(TASK_SWITCH_MODE_DE)
    deferred_token taskSwitchToken = INVALID_DEFERRED_TOKEN;
#else // TASK_SWITCH_MODE_DE
    uint16_t task_switch_timer = 0;
#endif // defined(TASK_SWITCH_MODE_DE)
bool mod_registered;

uint8_t task_switch_get_active_config_id(void) {
    #if defined(TASK_SWITCH_NOEEPROM) && defined(TASK_SWITCH_DISABLE_OS_DETECTION)
        return TASK_SWITCH_DEFAULT_MODE;
    #endif

    #if !defined(TASK_SWITCH_NOEEPROM)
        if(task_switch_config.use_os_detection){
    #endif
            switch (detected_host_os()) {
                case OS_MACOS:
                case OS_IOS:
                    return task_switch_config.macos_conf;
                case OS_WINDOWS:
                    return task_switch_config.windows_conf;
                case OS_LINUX:
                    return task_switch_config.linux_conf;
                case OS_UNSURE:
                default:
                    return task_switch_config.unknownos_conf;
            }
    #if !defined(TASK_SWITCH_NOEEPROM)
        } // end of if statement from above
        else{
            return task_switch_config.manual_conf;
        }
    #endif
}

uint16_t task_switch_get_delay(void){
    #if defined(TASK_SWITCH_NOEEPROM)
        return delay;
    #else
        return task_switch_config.delay;
    #endif
}

void task_switch_set_delay(uint16_t newdelay){
    dprintf("set_delay: %d\n", newdelay);
    // this is kinda pointless- it won't survive a restart.
    #if defined(TASK_SWITCH_NOEEPROM)
        delay = newdelay;
    #else
        task_switch_config.delay = newdelay;
        eeconfig_flag_task_switch(true);
    #endif
}

void task_switch_reset(void){
    mod_registered = false;
    task_switch_osconf_t tsconfig = task_switch_get_config_object(task_switch_get_active_config_id());
    unregister_mods(tsconfig.mod);
}

#if defined(TASK_SWITCH_MODE_DE)
    uint32_t task_switch_deferred_task(uint32_t trigger_time, void *cb_arg) {
        task_switch_reset();
        taskSwitchToken = INVALID_DEFERRED_TOKEN;
        dprintf("task_switch_deferred_task\n");
        return 0;
    }
#else // TASK_SWITCH_MODE_DE
    void housekeeping_task_task_switch(void) {
        if (mod_registered) {
            if (timer_elapsed(task_switch_timer) > task_switch_get_delay()) {
                task_switch_reset();
                dprintf("task_switch timer elapsed\n");
            }
        }
    }
#endif // TASK_SWITCH_MODE_DE

void task_switch_press(bool reverse) {
    task_switch_osconf_t tsconfig = task_switch_get_config_object(task_switch_get_active_config_id());
    #if defined(TASK_SWITCH_MODE_DE)
        if (taskSwitchToken != INVALID_DEFERRED_TOKEN) {
            cancel_deferred_exec(taskSwitchToken);
        }
    #endif // TASK_SWITCH_MODE_DE
    if (!(get_mods() & tsconfig.mod)){
        dprintf("task_switch sends mod: %d\n", tsconfig.mod);
        mod_registered = true;
        register_mods(tsconfig.mod);
    }
    uint16_t sendkc = tsconfig.tap;
    if(reverse){
        // Remove the standard `mod` from the `mod_r`
        // shift calculated mod left 8 bits to create  e.g. `S(KC_NO)`
        // then add `tap`
        sendkc = (((tsconfig.mod_r ^ tsconfig.mod) & tsconfig.mod_r) << 8) | tsconfig.tap_r;
    }
    dprintf("task_switch press %d\n", sendkc);
    tap_code16(sendkc);
}

void task_switch_release(void) {
    dprintf("task_switch release\n");
    if(mod_registered){
        #if defined(TASK_SWITCH_MODE_DE)
            taskSwitchToken = defer_exec(task_switch_get_delay(), task_switch_deferred_task, NULL);
        #else // TASK_SWITCH_MODE_DE
            task_switch_timer = timer_read();
        #endif // TASK_SWITCH_MODE_DE
    }
}

bool process_record_task_switch(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_task_switch_kb(keycode, record)) {
        return false;
    }
    if (record->event.pressed) {
        switch (keycode) {
            case COMMUNITY_MODULE_TASK_SWITCH_NEXT:
                task_switch_press(false);
                return false;
            case COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS:
                task_switch_press(true);
                return false;
        }
    }
    else{
        switch (keycode) {
            case COMMUNITY_MODULE_TASK_SWITCH_NEXT:
            case COMMUNITY_MODULE_TASK_SWITCH_PREVIOUS:
                task_switch_release();
                return false;
        }
    }
    return true;
}
