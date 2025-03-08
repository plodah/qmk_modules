
#include "deferred_exec.h"

#if !defined(MSJIGGLER_INTRO_TIMEOUT)
  #define MSJIGGLER_INTRO_TIMEOUT 1200
#endif

#if !defined(MSJIGGLER_INDICATOR_HSV)
    #define MSJIGGLER_INDICATOR_HSV HSV_ORANGE
#endif // !defined(MSJIGGLER_INDICATOR_HSV)

#if !defined(MSJIGGLER_INDICATOR_HSV)
    #define MSJIGGLER_INDICATOR_HSV HSV_ORANGE
#endif // !defined(MSJIGGLER_INDICATOR_HSV)

static deferred_token msJigMainToken = INVALID_DEFERRED_TOKEN;
static deferred_token msJigIntroToken = INVALID_DEFERRED_TOKEN;
static deferred_token msJigIntroTimerToken = INVALID_DEFERRED_TOKEN;
static report_mouse_t msJigReport = {0};

// Deltas only work if the length of the array is a power of 2.
static int8_t circledeltas[32] = {0,-1,-2,-2,-3,-3,-4,-4,-4,-4,-3,-3,-2,-2,-1,0,0,1,2,2,3,3,4,4,4,4,3,3,2,2,1,0};
static int8_t subtledeltas[16] = {1,-1,1,1,-2,2,-2,-2,2,-2,2,2,-1,1,-1,-1};
static int8_t squaredeltas[16] = {1,1,1,1,0,0,0,0,-1,-1,-1,-1,0,0,0,0};

void jiggler_end(void);
void jiggler_start(void);
void jiggler_toggle(void);
void jiggle_delay(uint16_t delay_sec);
