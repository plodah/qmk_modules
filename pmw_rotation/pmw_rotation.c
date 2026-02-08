#include QMK_KEYBOARD_H
#include "pmw_rotation.h"

#ifdef ROTATIONAL_TRANSFORM_ANGLE
    int8_t pmwrotation = ROTATIONAL_TRANSFORM_ANGLE;
#else
    int8_t pmwrotation = 0;
#endif // ROTATIONAL_TRANSFORM_ANGLE

int8_t pmw_rotation_get_sensor (void) {
    return pmw33xx_read(0, REG_Angle_Tune);
}

void pmw_rotation_set_sensor (int8_t cpi) {
    pmw33xx_write(0, REG_Angle_Tune, pmwrotation);
}

int8_t pmw_rotation_get_config (void) {
    return pmwrotation;
}

void pmw_rotation_set_config (bool absolute, int8_t value) {
    int16_t temp_pmwrotation = pmwrotation;
    if (absolute){ temp_pmwrotation = value; }
    else { temp_pmwrotation += value; }
    pmwrotation = CONSTRAIN(temp_pmwrotation, -PMW_ROTATION_LIMIT, PMW_ROTATION_LIMIT);
    dprintf("set pmwrotation:%d\n", -pmwrotation);
}

void pmw_rotation_config_to_sensor (void) {
    pmw_rotation_set_sensor(pmwrotation);
}

void pmw_rotation_sensor_to_config (void) {
    pmwrotation = pmw_rotation_get_sensor();
}

bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed){
        dprintf("process_record_pmw_rotation\n");
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
