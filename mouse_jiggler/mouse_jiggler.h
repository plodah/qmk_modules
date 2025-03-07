
#include "deferred_exec.h"

static deferred_token msJigMainToken = INVALID_DEFERRED_TOKEN;
static deferred_token msJigIntroToken = INVALID_DEFERRED_TOKEN;
static deferred_token msJigIntroTimerToken = INVALID_DEFERRED_TOKEN;
static report_mouse_t msJigReport = {0};

// Deltas only work if the length of the array is a power of 2.
static int8_t circledeltas[32] = {0,-1,-2,-2,-3,-3,-4,-4,-4,-4,-3,-3,-2,-2,-1,0,0,1,2,2,3,3,4,4,4,4,3,3,2,2,1,0};
static int8_t subtledeltas[16] = {1,-1,1,1,-2,2,-2,-2,2,-2,2,2,-1,1,-1,-1};
static int8_t squaredeltas[16] = {1,1,1,1,0,0,0,0,-1,-1,-1,-1,0,0,0,0};
