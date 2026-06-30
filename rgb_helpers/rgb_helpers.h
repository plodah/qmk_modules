#pragma once
#if defined(RGB_MATRIX_ENABLE)
    #include "rgb_matrix.h"
#endif // RGB_MATRIX_ENABLE
#if defined(RGBLIGHT_ENABLE)
    #include "rgblight.h"
#endif // RGBLIGHT_ENABLE

hsv_t rgbhelpers_limit(hsv_t currenthsv, hsv_t target_hsv, uint8_t minval);
hsv_t rgbhelpers_limit_ind(hsv_t currenthsv, uint8_t target_h, uint8_t target_s, uint8_t target_v, uint8_t minval);
