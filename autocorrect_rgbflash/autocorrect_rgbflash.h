#pragma once

#include QMK_KEYBOARD_H
#if defined(RGB_MATRIX_ENABLE)
#include "rgb_matrix.h"
#endif // RGB_MATRIX_ENABLE

#if defined(RGBLIGHT_ENABLE)
#include "rgblight.h"
#endif // RGBLIGHT_ENABLE

#include "rgb_helpers.h"

// #define AUTOCORRECT_RGBFLASH_FORCE_NDE  // define for degug purposes
#if defined(DEFERRED_EXEC_ENABLE) && (!defined(AUTOCORRECT_RGBFLASH_FORCE_NDE))
    #include "deferred_exec.h"
    #define AUTOCORRECT_RGBFLASH_MODE_DE
#endif // DEFERRED_EXEC_ENABLE

#ifndef AUTOCORRECT_RGBFLASH_DURATION
    #define AUTOCORRECT_RGBFLASH_DURATION 200
#endif //AUTOCORRECT_RGBFLASH_DURATION

#ifndef AUTOCORRECT_RGBFLASH_BLINKCOUNT
    #define AUTOCORRECT_RGBFLASH_BLINKCOUNT 3
#endif // AUTOCORRECT_RGBFLASH_BLINKCOUNT

#ifndef AUTOCORRECT_RGBFLASH_COLOR
    #define AUTOCORRECT_RGBFLASH_COLOR HSV_RED
#endif //AUTOCORRECT_RGBFLASH_COLOR

#ifndef AUTOCORRECT_RGBFLASH_MINVAL
    #define AUTOCORRECT_RGBFLASH_MINVAL 85
#endif // AUTOCORRECT_RGBFLASH_MINVAL

#if !defined(AUTOCORRECT_RGBFLASH_MODE_DE)
    void housekeeping_task_autocorrect_rgbflash(void);
#endif // #if !defined(AUTOCORRECT_RGBFLASH_MODE_DE)

bool apply_autocorrect_autocorrect_rgbflash(uint8_t backspaces, const char *str, char *typo, char *correct);
