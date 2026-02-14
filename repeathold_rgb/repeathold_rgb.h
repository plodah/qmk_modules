#pragma once

#if defined(RGB_MATRIX_ENABLE)
    #include "rgb_matrix.h"
    typedef struct {
        bool vald;
        bool valu;
        bool hued;
        bool hueu;
        bool satd;
        bool satu;
        bool spdd;
        bool spdu;
    } rgb_repeathold_rm_state_t;
    rgb_repeathold_rm_state_t rm_state;
#endif // RGB_MATRIX_ENABLE

#if defined(RGBLIGHT_ENABLE)
    #include "rgblight.h"
    typedef struct {
        bool vald;
        bool valu;
        bool hued;
        bool hueu;
        bool satd;
        bool satu;
        bool spdd;
        bool spdu;
    } rgb_repeathold_ug_state_t;
    rgb_repeathold_ug_state_t ug_state;
#endif // RGBLIGHT_ENABLE

#if !defined(PLODAH_REPEATHOLD_RATE)
  #define PLODAH_REPEATHOLD_RATE 125
#endif // !defined(PLODAH_REPEATHOLD_RATE)
