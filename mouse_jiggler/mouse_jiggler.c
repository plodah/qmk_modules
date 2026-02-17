// cannibalised
// https://getreuer.info/posts/keyboards/macros3/index.html#a-mouse-jiggler to
// make this.

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include "mouse_jiggler.h"

report_mouse_t msJigReport = {0};
deferred_token msJigMainToken = INVALID_DEFERRED_TOKEN;
deferred_token msJigIntroToken = INVALID_DEFERRED_TOKEN;
deferred_token msJigIntroTimerToken = INVALID_DEFERRED_TOKEN;

uint8_t jiggler_get_state (void) {
    if (msJigMainToken != INVALID_DEFERRED_TOKEN){
        if(msJigIntroToken != INVALID_DEFERRED_TOKEN){
            return 2;
        } else {
            return 1;
        }
    }
    return 0;
}

uint32_t jiggler_pattern(int8_t deltas[], int8_t numdeltas, int8_t phasefraction, int8_t scalex, int8_t scaley, bool randomdelay, int16_t basedelay) {
    static uint8_t phase = 0;
    uint32_t delay;
    msJigReport.x = scalex * deltas[phase];
    msJigReport.y = scaley * deltas[(phase + (numdeltas / phasefraction)) & (numdeltas - 1)];
    host_mouse_send(&msJigReport);
    phase = (phase + 1) & (numdeltas - 1);
    if(randomdelay){
        delay = basedelay + deltas[phase] * basedelay / 4 + phase * basedelay / 10;
    } else {
        delay = basedelay;
    }
    if(delay>=10000){
        dprintf("msjg; next: %ds\n", (uint16_t)(delay/1000));
    }
    else{
        dprintf("msjg; next: %dms\n", (uint16_t)(delay));
    }
    return delay;
}

void jiggler_intro_end(void) {
    if (msJigIntroToken != INVALID_DEFERRED_TOKEN) {
        dprintf("jiggle end of intro/outro\n");
        cancel_deferred_exec(msJigIntroToken);
        msJigIntroToken = INVALID_DEFERRED_TOKEN;
    }
    if (msJigIntroTimerToken != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(msJigIntroTimerToken);
        msJigIntroTimerToken = INVALID_DEFERRED_TOKEN;
    }
}

// Deltas only work if the length of the array is a power of 2.
int8_t circledeltas[32] = {0,  -1, -2, -2, -3, -3, -4, -4, -4, -4, -3,
                           -3, -2, -2, -1, 0,  0,  1,  2,  2,  3,  3,
                           4,  4,  4,  4,  3,  3,  2,  2,  1,  0};
int8_t subtledeltas[16] = {1, -1, 1, 1, -2, 2, -2, -2,
                           2, -2, 2, 2, -1, 1, -1, -1};
int8_t squaredeltas[16] = {1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0};

// jiggler_pattern( deltas[], numdeltas, phasefraction, scalex, scaley, randomdelay, basedelay )
uint32_t jiggler_circle(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, -2, 2, 0, 64);
}

uint32_t jiggler_circle_small(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, -1, 1, 0, 24);
}

uint32_t jiggler_circle_ccw(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, 2, 2, 0, 64);
}

uint32_t jiggler_circle_ccw_small(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, 1, 1, 0, 24);
}

uint32_t jiggler_square(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(squaredeltas, 16, 4, 2, 2, 0, 64);
}


uint32_t jiggler_figure(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, 4, 4, 0, 64);
}

uint32_t jiggler_subtle(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(subtledeltas, 16, 4, 1, 1, 1, 16384);
}

uint32_t jiggler_xline(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, 1, 0, 0, 24);
}

uint32_t jiggler_yline(uint32_t trigger_time, void *cb_arg) {
    return jiggler_pattern(circledeltas, 32, 4, 0, 1, 0, 24);
}

uint32_t jiggler_introtimer(uint32_t trigger_time, void *cb_arg) {
    jiggler_intro_end();
    return 0;
}

void jiggler_end(void) {
    dprintf("jiggler_end\n");
    cancel_deferred_exec(msJigMainToken);
    msJigReport = (report_mouse_t){}; // Clear the mouse.
    host_mouse_send(&msJigReport);
    #if !defined(MSJIGGLER_NOINTRO)
        #if MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_SUBTLE
            msJigIntroToken = defer_exec(1, jiggler_subtle, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_XLINE
            msJigIntroToken = defer_exec(1, jiggler_xline, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_YLINE
            msJigIntroToken = defer_exec(1, jiggler_yline, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_CIRCLE
            msJigIntroToken = defer_exec(1, jiggler_circle, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_CIRCLESMALL
            msJigIntroToken = defer_exec(1, jiggler_circle_small, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_CIRCLECCW
            msJigIntroToken = defer_exec(1, jiggler_circle_ccw, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_CIRCLECCWSMALL
            msJigIntroToken = defer_exec(1, jiggler_circle_ccw_small, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_FIGURE
            msJigIntroToken = defer_exec(1, jiggler_figure, NULL);
        #elif MSJIGGLER_PATTERN_ENDING == MSJIGGLER_PATTERN_SQUARE
            msJigIntroToken = defer_exec(1, jiggler_square, NULL);
        #endif // MSJIGGLER_PATTERN_ENDING

        #if defined(MSJIGGLER_INTRO_TIMEOUT)
            msJigIntroTimerToken = defer_exec(MSJIGGLER_INTRO_TIMEOUT, jiggler_introtimer, NULL);
        #endif // MSJIGGLER_INTRO_TIMEOUT
    #endif // !defined(MSJIGGLER_NOINTRO)
    msJigMainToken = INVALID_DEFERRED_TOKEN;
}

void jiggler_start(void) {
    #if MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_SUBTLE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_SUBTLE \n");
        msJigMainToken = defer_exec(1, jiggler_subtle, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_XLINE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_XLINE \n");
        msJigMainToken = defer_exec(1, jiggler_xline, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_YLINE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_YLINE \n");
        msJigMainToken = defer_exec(1, jiggler_yline, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_CIRCLE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_CIRCLE \n");
        msJigMainToken = defer_exec(1, jiggler_circle, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_CIRCLESMALL
        dprintf("jiggler_start: MSJIGGLER_PATTERN_CIRCLESMALL \n");
        msJigMainToken = defer_exec(1, jiggler_circle_small, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_CIRCLECCW
        dprintf("jiggler_start: MSJIGGLER_PATTERN_CIRCLECCW \n");
        msJigMainToken = defer_exec(1, jiggler_circle_ccw, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_CIRCLECCWSMALL
        dprintf("jiggler_start: MSJIGGLER_PATTERN_CIRCLECCWSMALL \n");
        msJigMainToken = defer_exec(1, jiggler_circle_ccw_small, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_FIGURE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_FIGURE \n");
        msJigMainToken = defer_exec(1, jiggler_figure, NULL);
    #elif MSJIGGLER_PATTERN == MSJIGGLER_PATTERN_SQUARE
        dprintf("jiggler_start: MSJIGGLER_PATTERN_SQUARE \n");
        msJigMainToken = defer_exec(1, jiggler_square, NULL);
    #endif // MSJIGGLER_PATTERN

    #if !defined(MSJIGGLER_NOINTRO)
        #if MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_SUBTLE
            dprintf("intro: MSJIGGLER_PATTERN_SUBTLE \n");
            msJigIntroToken = defer_exec(1, jiggler_subtle, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_XLINE
            dprintf("intro: MSJIGGLER_PATTERN_XLINE \n");
            msJigIntroToken = defer_exec(1, jiggler_xline, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_YLINE
            dprintf("intro: MSJIGGLER_PATTERN_YLINE \n");
            msJigIntroToken = defer_exec(1, jiggler_yline, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_CIRCLE
            dprintf("intro: MSJIGGLER_PATTERN_CIRCLE \n");
            msJigIntroToken = defer_exec(1, jiggler_circle, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_CIRCLESMALL
            dprintf("intro: MSJIGGLER_PATTERN_CIRCLESMALL \n");
            msJigIntroToken = defer_exec(1, jiggler_circle_small, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_CIRCLECCW
            dprintf("intro: MSJIGGLER_PATTERN_CIRCLECCW \n");
            msJigIntroToken = defer_exec(1, jiggler_circle_ccw, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_CIRCLECCWSMALL
            dprintf("intro: MSJIGGLER_PATTERN_CIRCLECCWSMALL \n");
            msJigIntroToken = defer_exec(1, jiggler_circle_ccw_small, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_FIGURE
            dprintf("intro: MSJIGGLER_PATTERN_FIGURE \n");
            msJigIntroToken = defer_exec(1, jiggler_figure, NULL);
        #elif MSJIGGLER_PATTERN_INTRO == MSJIGGLER_PATTERN_SQUARE
            dprintf("intro: MSJIGGLER_PATTERN_SQUARE \n");
            msJigIntroToken = defer_exec(1, jiggler_square, NULL);
        #endif // MSJIGGLER_PATTERN_INTRO
        #if defined(MSJIGGLER_INTRO_TIMEOUT)
            dprintf("intro timer: %dms \n", MSJIGGLER_INTRO_TIMEOUT);
            msJigIntroTimerToken = defer_exec(MSJIGGLER_INTRO_TIMEOUT, jiggler_introtimer, NULL);
        #endif // MSJIGGLER_INTRO_TIMEOUT
    #endif // !defined(MSJIGGLER_NOINTRO)
}

void jiggler_toggle(void) {
    dprintf("jiggler_toggle\n");
    jiggler_intro_end();
    if (jiggler_get_state()) {
        jiggler_end();
    } else {
        jiggler_start();
    }
}

void jiggle_delay(uint32_t delay_sec) {
    if (jiggler_get_state()) {
        // dprintf("delay the jiggles\n");
        extend_deferred_exec(msJigMainToken, delay_sec * 1000);
    }
}

bool process_record_mouse_jiggler(uint16_t keycode, keyrecord_t *record) {
    if (
        #if defined(MSJIGGLER_AUTOSTOP)
            jiggler_get_state() ||
        #endif // MSJIGGLER_AUTOSTOP
        keycode == COMMUNITY_MODULE_MOUSE_JIGGLER_TOGGLE &&
        record->event.pressed
    ) {
        jiggler_toggle();
        jiggle_delay(MSJIGGLER_BACKOFF);
        return false;
    }

    // delay for MSJIGGLER_BACKOFF seconds when any key is pressed.
    // avoids simulated action interfering with real actions.
    jiggle_delay(MSJIGGLER_BACKOFF);
    return true;
}

report_mouse_t pointing_device_task_mouse_jiggler(report_mouse_t mouse_report) {
    if (mouse_report.x || mouse_report.y || mouse_report.h || mouse_report.v ) {
        jiggle_delay(MSJIGGLER_BACKOFF);
    }
    return mouse_report;
}
