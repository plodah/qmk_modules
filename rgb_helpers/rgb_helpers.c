#include QMK_KEYBOARD_H
#include "rgb_helpers.h"

hsv_t rgbhelpers_limit(hsv_t currenthsv, hsv_t target_hsv, uint8_t minval){
    hsv_t output = target_hsv;
    if(minval > currenthsv.v){
        output.v = minval;
    }
    else if(target_hsv.v > currenthsv.v ){
        output.v = currenthsv.v;
    }
    return output;
}

hsv_t rgbhelpers_limit_ind(hsv_t currenthsv, uint8_t target_h, uint8_t target_s, uint8_t target_v, uint8_t minval){
    hsv_t target_hsv = {target_h, target_s, target_v};
    return rgbhelpers_limit(currenthsv, target_hsv, minval);
}
