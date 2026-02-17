#pragma once

#include QMK_KEYBOARD_H
#include <stdint.h>
#include "deferred_exec.h"

uint8_t jiggler_get_state(void);
void jiggler_end(void);
void jiggler_start(void);
void jiggler_toggle(void);
void jiggle_delay(uint32_t delay_sec);

#if (!defined(MSJIGGLER_NOINTRO)) && (!defined(MSJIGGLER_INTRO_TIMEOUT))
    #define MSJIGGLER_INTRO_TIMEOUT 1000
#endif // MSJIGGLER_NOINTRO

#define MSJIGGLER_PATTERN_SUBTLE 1
#define MSJIGGLER_PATTERN_XLINE 2
#define MSJIGGLER_PATTERN_YLINE 3
#define MSJIGGLER_PATTERN_CIRCLE 4
#define MSJIGGLER_PATTERN_CIRCLESMALL 5
#define MSJIGGLER_PATTERN_CIRCLECCW 6
#define MSJIGGLER_PATTERN_CIRCLECCWSMALL 7
#define MSJIGGLER_PATTERN_FIGURE 8
#define MSJIGGLER_PATTERN_SQUARE 9

#if ! defined(MSJIGGLER_PATTERN)
    #define MSJIGGLER_PATTERN MSJIGGLER_PATTERN_SUBTLE
#endif // defined(MSJIGGLER_PATTERN)

#if ! defined(MSJIGGLER_PATTERN_INTRO)
    #define MSJIGGLER_PATTERN_INTRO MSJIGGLER_PATTERN_CIRCLESMALL
#endif // defined(MSJIGGLER_PATTERN)

#if ! defined(MSJIGGLER_PATTERN_ENDING)
    #define MSJIGGLER_PATTERN_ENDING MSJIGGLER_PATTERN_CIRCLECCWSMALL
#endif // defined(MSJIGGLER_PATTERN)

#if ! defined(MSJIGGLER_BACKOFF)
    #define MSJIGGLER_BACKOFF 30
#endif // defined(MSJIGGLER_BACKOFF)
