#pragma once
#include "deferred_exec.h"
#include <stdint.h>

#if (!defined(MSJIGGLER_NOINTRO)) && (!defined(MSJIGGLER_INTRO_TIMEOUT))
    #define MSJIGGLER_INTRO_TIMEOUT 1400
#endif // MSJIGGLER_NOINTRO

deferred_token msJigMainToken;
deferred_token msJigIntroToken;
deferred_token msJigIntroTimerToken;

void jiggler_end(void);
void jiggler_start(void);
void jiggler_toggle(void);
void jiggle_delay(uint16_t delay_sec);
