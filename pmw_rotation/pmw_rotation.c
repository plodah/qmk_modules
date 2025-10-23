#include QMK_KEYBOARD_H
#include "pmw_rotation.h"

void pmw_rotation_act (bool absolute, int16_t value) {
    if (absolute){ pmwrotation = value; }
    else { pmwrotation += value; }
    while(! (-128 <= pmwrotation && pmwrotation <= 128)){
        if (pmwrotation > 128){pmwrotation -= 256 + value;}
        if (pmwrotation < -128){pmwrotation += 256 - value;}
    }
    dprintf("set pmwrotation:%d\n", pmwrotation);
    pmw33xx_write(0, REG_Angle_Tune, CONSTRAIN(pmwrotation, -127, 127));
}

bool process_record_pmw_rotation (uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed){
        dprintf("process_record_pmw_rotation");
        switch (keycode) {
            case COMMUNITY_MODULE_PMW_ROTATE_CCW:
                dprintf(" PMW_ROTATE_CCW\n");
                pmw_rotation_act(false, -PMW_ROTATION_STEP_SIZE);
                return false;
            case COMMUNITY_MODULE_PMW_ROTATE_CW:
                dprintf(" PMW_ROTATE_CW\n");
                pmw_rotation_act(false, PMW_ROTATION_STEP_SIZE);
                return false;
            case COMMUNITY_MODULE_PMW_ROTATE_RESET:
                dprintf(" PMW_ROTATE_RST\n");
                pmw_rotation_act(true, ROTATIONAL_TRANSFORM_ANGLE);
                return false;
            default:
                return true;
        }
    }
    return true;
}
