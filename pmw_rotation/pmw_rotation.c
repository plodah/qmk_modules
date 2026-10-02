#include QMK_KEYBOARD_H
#include "pmw_rotation.h"
#include "drivers/sensors/pmw33xx_common.h"

#if defined(PMW_ROTATION_NOEEPROM)
    int8_t pointer_rotation_value;
#else
    #include "eeconfig.h"
    pmw_rotation_config_t pmw_rotation_config;
    pmw_rotation_config_t pmw_rotation_default_config = {
        #ifdef ROTATIONAL_TRANSFORM_ANGLE
            .rotation = ROTATIONAL_TRANSFORM_ANGLE,
        #else // ROTATIONAL_TRANSFORM_ANGLE
            .rotation = 0,
        #endif // ROTATIONAL_TRANSFORM_ANGLE
    };

    _Static_assert(sizeof(pmw_rotation_config_t) <= EECONFIG_MODULE_PMW_ROTATION_DATA_SIZE, "EECONFIG_MODULE_PMW_ROTATION_DATA_SIZE is too small");

    void eeconfig_read_pmw_rotation(pmw_rotation_config_t *value) {
        eeconfig_read_pmw_rotation_datablock(value, 0, sizeof(pmw_rotation_config_t));
    }

    void eeconfig_update_pmw_rotation(pmw_rotation_config_t *value) {
        eeconfig_update_pmw_rotation_datablock(value, 0, sizeof(pmw_rotation_config_t));
    }

    EECONFIG_DEBOUNCE_HELPER(pmw_rotation, pmw_rotation_config);

    void keyboard_post_init_pmw_rotation(void) {
        eeconfig_init_pmw_rotation();
        pmw_rotation_config_to_sensor();
    }

    void eeconfig_init_pmw_rotation_datablock(void) {
        pmw_rotation_config = pmw_rotation_default_config;
        eeconfig_flush_pmw_rotation(true);
    }

    void housekeeping_task_pmw_rotation(void) {
        eeconfig_flush_pmw_rotation_task(1000);
    }
#endif

int8_t pmw_rotation_get_sensor (void) {
    return pmw33xx_read(0, REG_Angle_Tune);
}

void pmw_rotation_set_sensor (int8_t cpi) {
    pmw33xx_write(0, REG_Angle_Tune, pmw_rotation_get_config());
}

int8_t pmw_rotation_get_config (void) {
    #if defined(PMW_ROTATION_NOEEPROM)
        return pointer_rotation_value;
    #else
        return pmw_rotation_config.rotation;
    #endif
}

pmw_rotation_uconfig_t pmw_rotation_get_uconfig (void) {
    pmw_rotation_uconfig_t output = {
        .rotation = abs(pmw_rotation_get_config()),
        .ccw = pmw_rotation_get_config()<0,
    };
    return output;
}

void pmw_rotation_set_config (bool absolute, int8_t value) {
    int16_t working_val = pmw_rotation_get_config();
    working_val = absolute ? value : (working_val+value);
    working_val = CONSTRAIN(working_val, -PMW_ROTATION_LIMIT, PMW_ROTATION_LIMIT);
    #if defined(PMW_ROTATION_NOEEPROM)
        pointer_rotation_value = working_val;
    #else
        pmw_rotation_config.rotation = working_val;
        eeconfig_flag_pmw_rotation(true);
    #endif
    dprintf("set pmwrotation:%d\n", working_val);
}

void pmw_rotation_set_uconfig (bool absolute, pmw_rotation_uconfig_t value){
    pmw_rotation_set_config(absolute, value.ccw ? -value.rotation : value.rotation);
}

void pmw_rotation_config_to_sensor (void) {
    pmw_rotation_set_sensor(pmw_rotation_get_config());
}

void pmw_rotation_sensor_to_config (void) {
    pmw_rotation_set_config(true, pmw_rotation_get_sensor());
}

bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed){
        // dprintf("process_record_pmw_rotation\n");
        switch (keycode) {
            case COMMUNITY_MODULE_PMW_ROTATE_CCW:
                pmw_rotation_set_config(false, PMW_ROTATION_STEP_SIZE);
                pmw_rotation_config_to_sensor();
                dprintf(" PMW_ROTATE_CCW\n");
                return false;
            case COMMUNITY_MODULE_PMW_ROTATE_CW:
                pmw_rotation_set_config(false, -PMW_ROTATION_STEP_SIZE);
                pmw_rotation_config_to_sensor();
                dprintf(" PMW_ROTATE_CW\n");
                return false;
            case COMMUNITY_MODULE_PMW_ROTATE_RESET:
                pmw_rotation_set_config(true, ROTATIONAL_TRANSFORM_ANGLE);
                pmw_rotation_config_to_sensor();
                dprintf(" PMW_ROTATE_RST\n");
                return false;
            default:
                return true;
        }
    }
    return true;
}
