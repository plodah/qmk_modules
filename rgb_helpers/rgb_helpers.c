#include "rgb_helpers.h"

HSV rgbhelpers_limit(HSV currenthsv, HSV target_hsv, uint8_t minval){
    HSV output = target_hsv;
    if(minval > currenthsv.v){
        output.v = minval;
    }
    else if(target_hsv.v > currenthsv.v ){
        output.v = currenthsv.v;
    }
    return output;
}

HSV rgbhelpers_limit_ind(HSV currenthsv, uint8_t target_h, uint8_t target_s, uint8_t target_v, uint8_t minval){
    HSV target_hsv = {target_h, target_s, target_v};
    return rgbhelpers_limit(currenthsv, target_hsv, minval);
}
