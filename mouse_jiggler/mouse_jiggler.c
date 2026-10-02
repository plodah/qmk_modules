// cannibalised
// https://getreuer.info/posts/keyboards/macros3/index.html#a-mouse-jiggler to
// make this.

#include QMK_KEYBOARD_H
#include "mouse_jiggler.h"

#if defined(MSJIGGLER_NOEEPROM)
    // Are there any variables used in this case?
#else
    #include "eeconfig.h"
    typedef struct mouse_jiggler_config_t {
        #ifdef MSJIGGLER_BACKOFF_IN_EEPROM
            uint16_t    backoff     :16
        #endif
        uint8_t     pattern     :4;
        uint8_t     pattern_int :4;
        uint8_t     pattern_out :4;
        bool        state       :1;
    } mouse_jiggler_config_t;
    mouse_jiggler_config_t mouse_jiggler_config;
    mouse_jiggler_config_t mouse_jiggler_default_config = {
        #ifdef MSJIGGLER_BACKOFF_IN_EEPROM
            .backoff        = MSJIGGLER_BACKOFF,
        #endif // MSJIGGLER_BACKOFF_IN_EEPROM
        .pattern        = MSJIGGLER_PATTERN,
        .pattern_int    = MSJIGGLER_PATTERN_INTRO,
        .pattern_out    = MSJIGGLER_PATTERN_ENDING,
        .state          = false,
    };

    _Static_assert(sizeof(mouse_jiggler_config_t) <= EECONFIG_MODULE_MOUSE_JIGGLER_DATA_SIZE, "EECONFIG_MODULE_MOUSE_JIGGLER_DATA_SIZE is too small");

    void eeconfig_read_mouse_jiggler(mouse_jiggler_config_t *value) {
        eeconfig_read_mouse_jiggler_datablock(value, 0, sizeof(mouse_jiggler_config_t));
    }

    void eeconfig_update_mouse_jiggler(mouse_jiggler_config_t *value) {
        eeconfig_update_mouse_jiggler_datablock(value, 0, sizeof(mouse_jiggler_config_t));
    }

    EECONFIG_DEBOUNCE_HELPER(mouse_jiggler, mouse_jiggler_config);

    void keyboard_post_init_mouse_jiggler(void) {
        eeconfig_init_mouse_jiggler();
        if(jiggler_get_state()){ jiggler_start(); }
    }

    void eeconfig_init_mouse_jiggler_datablock(void) {
        mouse_jiggler_config = mouse_jiggler_default_config;
        eeconfig_flush_mouse_jiggler(true);
    }

    void housekeeping_task_mouse_jiggler(void) {
        eeconfig_flush_mouse_jiggler_task(1000);
    }

#endif // MSJIGGLER_NOEEPROM

report_mouse_t msJigReport = {0};
deferred_token msJigMainToken = INVALID_DEFERRED_TOKEN;
deferred_token msJigIntroToken = INVALID_DEFERRED_TOKEN;
deferred_token msJigIntroTimerToken = INVALID_DEFERRED_TOKEN;

uint8_t jiggler_get_true_state (void) {
    if (msJigMainToken != INVALID_DEFERRED_TOKEN){
        if(msJigIntroToken != INVALID_DEFERRED_TOKEN){
            return MSJIGGLER_STATE_RUNINTRO;
        } else {
            return MSJIGGLER_STATE_RUNNING;
        }
    }
    return MSJIGGLER_STATE_OFF;
}

bool jiggler_get_state (void) {
    #if defined(MSJIGGLER_NOEEPROM)
        return (bool)jiggler_get_true_state();
    #else
        return mouse_jiggler_config.state;
    #endif
}

uint8_t jiggler_get_config_pattern(void){
    #if defined(MSJIGGLER_NOEEPROM)
        return MSJIGGLER_PATTERN;
    #else
        return mouse_jiggler_config.pattern;
    #endif
}

uint8_t jiggler_get_config_pattern_int(void){
    #if defined(MSJIGGLER_NOEEPROM)
        return MSJIGGLER_PATTERN_INTRO;
    #else
        return mouse_jiggler_config.pattern_int;
    #endif
}

uint8_t jiggler_get_config_pattern_out(void){
    #if defined(MSJIGGLER_NOEEPROM)
        return MSJIGGLER_PATTERN_ENDING;
    #else
        return mouse_jiggler_config.pattern_out;
    #endif
}

void jiggler_set_state (bool newstate) {
    dprintf("jiggler_set_state(%d)\n", newstate);
    if(newstate){
        jiggler_start();
    }
    else{
        jiggler_end();
    }
    #if defined(MSJIGGLER_NOEEPROM)
    #else
        mouse_jiggler_config.state = newstate;
    #endif
    eeconfig_flag_mouse_jiggler(true);
}

void jiggler_toggle(void) {
    jiggler_set_state(!jiggler_get_state());
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

deferred_token jiggler_start_pattern(uint8_t pattern){
    switch(pattern){
        case MSJIGGLER_PATTERN_NONE:
            return INVALID_DEFERRED_TOKEN;
        case MSJIGGLER_PATTERN_SUBTLE:
            return defer_exec(1, jiggler_subtle, NULL);
        case MSJIGGLER_PATTERN_XLINE:
            return defer_exec(1, jiggler_xline, NULL);
        case MSJIGGLER_PATTERN_YLINE:
            return defer_exec(1, jiggler_yline, NULL);
        case MSJIGGLER_PATTERN_CIRCLE:
            return defer_exec(1, jiggler_circle, NULL);
        case MSJIGGLER_PATTERN_CIRCLESMALL:
            return defer_exec(1, jiggler_circle_small, NULL);
        case MSJIGGLER_PATTERN_CIRCLECCW:
            return defer_exec(1, jiggler_circle_ccw, NULL);
        case MSJIGGLER_PATTERN_CIRCLECCWSMALL:
            return defer_exec(1, jiggler_circle_ccw_small, NULL);
        case MSJIGGLER_PATTERN_FIGURE:
            return defer_exec(1, jiggler_figure, NULL);
        case MSJIGGLER_PATTERN_SQUARE:
            return defer_exec(1, jiggler_square, NULL);
    }
    return INVALID_DEFERRED_TOKEN;
}


void jiggler_end(void) {
    dprintf("jiggler_end\n");
    jiggler_intro_end();
    cancel_deferred_exec(msJigMainToken);
    msJigReport = (report_mouse_t){}; // Clear the mouse.
    host_mouse_send(&msJigReport);
    msJigIntroToken = jiggler_start_pattern(jiggler_get_config_pattern_out());
    msJigIntroTimerToken = defer_exec(MSJIGGLER_INTRO_TIMEOUT, jiggler_introtimer, NULL);
    msJigMainToken = INVALID_DEFERRED_TOKEN;
}

void jiggler_start(void) {
    jiggler_intro_end();
    msJigMainToken = jiggler_start_pattern(jiggler_get_config_pattern());
    msJigIntroToken = jiggler_start_pattern(jiggler_get_config_pattern_int());
    dprintf("intro timer: %dms \n", MSJIGGLER_INTRO_TIMEOUT);
    msJigIntroTimerToken = defer_exec(MSJIGGLER_INTRO_TIMEOUT, jiggler_introtimer, NULL);
}

void jiggle_delay(uint32_t delay_sec) {
    if (jiggler_get_true_state()) {
        // dprintf("delay the jiggles\n");
        extend_deferred_exec(msJigMainToken, delay_sec * 1000);
    }
}

bool process_record_mouse_jiggler(uint16_t keycode, keyrecord_t *record) {
    if (
        #if defined(MSJIGGLER_AUTOSTOP)
            jiggler_get_true_state() ||
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
