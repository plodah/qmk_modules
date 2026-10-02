#pragma once

typedef struct pmw_rotation_config_t {
    int8_t  rotation;
} pmw_rotation_config_t;
typedef struct pmw_rotation_uconfig_t {
    uint8_t     rotation:7;
    bool        ccw:1;
} pmw_rotation_uconfig_t;

#ifndef PMW_ROTATION_STEP_SIZE
    #define PMW_ROTATION_STEP_SIZE 15
#endif
#ifndef PMW_ROTATION_LIMIT
    #define PMW_ROTATION_LIMIT 127
#endif

int8_t pmw_rotation_get_config (void);
void pmw_rotation_set_config (bool absolute, int8_t value);

pmw_rotation_uconfig_t pmw_rotation_get_uconfig (void);
void pmw_rotation_set_uconfig (bool absolute, pmw_rotation_uconfig_t value);

void pmw_rotation_config_to_sensor (void);
void pmw_rotation_sensor_to_config (void);

bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record);
