#pragma once

int8_t pmw_rotation_get_config (void);
void pmw_rotation_set_config (bool absolute, int8_t value, bool apply);


typedef struct pmw_rotation_uconfig_t {
    uint8_t     rotation:7;
    bool        ccw:1;
} pmw_rotation_uconfig_t;
pmw_rotation_uconfig_t pmw_rotation_get_uconfig (void);
void pmw_rotation_set_uconfig (bool absolute, pmw_rotation_uconfig_t value, bool apply);

void pmw_rotation_config_to_sensor (void);
void pmw_rotation_sensor_to_config (void);

bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record);
