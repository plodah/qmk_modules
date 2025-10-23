#pragma once

#if defined(RGB_MATRIX_ENABLE)
    #include "rgb_matrix.h"
#endif // RGB_MATRIX_ENABLE
#if defined(RGBLIGHT_ENABLE)
    #include "rgblight.h"
#endif // RGBLIGHT_ENABLE

HSV rgbhelpers_limit(HSV currenthsv, HSV target_hsv, uint8_t minval);
HSV rgbhelpers_limit_ind(HSV currenthsv, uint8_t target_h, uint8_t target_s, uint8_t target_v, uint8_t minval);
