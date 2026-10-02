#pragma once

void task_switch_press(bool reverse);
void task_switch_release(void);

uint16_t task_switch_get_delay(void);
void task_switch_set_delay(uint16_t newdelay);

bool task_switch_get_os_detection_state(void);
void task_switch_set_os_detection_state(bool newstate);

void task_switch_set_eeconfig_key(bool configb, int8_t key, uint16_t newvalue);
uint16_t task_switch_get_eeconfig_key(bool configb, int8_t key);

uint8_t task_switch_get_eeconfig_configset(uint8_t configset);
void task_switch_set_eeconfig_configset(uint8_t configset, uint8_t newoption);

enum {
    TASK_SWITCH_CONFIG_KEY_MOD = 0,
    TASK_SWITCH_CONFIG_KEY_RMOD,
    TASK_SWITCH_CONFIG_KEY_TAP,
    TASK_SWITCH_CONFIG_KEY_RTAP, // 3
};

enum {
    TASK_SWITCH_CONFIG_SET_INVALID = 0,
    TASK_SWITCH_CONFIG_SET_WINDOWS,
    TASK_SWITCH_CONFIG_SET_MACOS,
    TASK_SWITCH_CONFIG_SET_CUSTOM_A,
    TASK_SWITCH_CONFIG_SET_CUSTOM_B, // 4
};

enum {
    TASK_SWITCH_OPTION_INVALID= 0,
    TASK_SWITCH_OPTION_WINDOWS,
    TASK_SWITCH_OPTION_MACOS,
    TASK_SWITCH_OPTION_LINUX,
    TASK_SWITCH_OPTION_UNKNOWNOS,
    TASK_SWITCH_OPTION_MANUAL,
};
