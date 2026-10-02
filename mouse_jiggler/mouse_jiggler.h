#pragma once

bool jiggler_get_state(void);
uint8_t jiggler_get_true_state(void);
bool jiggler_get_state(void);
void jiggler_set_state(bool newstate);
void jiggler_end(void);
void jiggler_start(void);
void jiggler_toggle(void);
void jiggle_delay(uint32_t delay_sec);

#define MSJIGGLER_INTRO_TIMEOUT 1000

enum jiggler_patterns {
    MSJIGGLER_PATTERN_NONE = 0,
    MSJIGGLER_PATTERN_SUBTLE,
    MSJIGGLER_PATTERN_XLINE,
    MSJIGGLER_PATTERN_YLINE,
    MSJIGGLER_PATTERN_CIRCLE,
    MSJIGGLER_PATTERN_CIRCLESMALL,
    MSJIGGLER_PATTERN_CIRCLECCW,
    MSJIGGLER_PATTERN_CIRCLECCWSMALL,
    MSJIGGLER_PATTERN_FIGURE,
    MSJIGGLER_PATTERN_SQUARE, // 9
};

enum jiggler_states {
    MSJIGGLER_STATE_OFF = 0,
    MSJIGGLER_STATE_RUNNING,
    MSJIGGLER_STATE_RUNINTRO,
};

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
