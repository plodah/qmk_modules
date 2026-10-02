#include QMK_KEYBOARD_H
#include "dragscroll_straighten.h"

#if defined(DRAGSCROLL_STRAIGHTEN_NOEEPROM)
    uint8_t sensitivity;
    bool    ds_str_state;
#else
    #include "eeconfig.h"
    typedef struct dragscroll_straighten_config_t {
        bool     state      : 1;
        uint8_t  sensitivity  : 7;
    } dragscroll_straighten_config_t;
    dragscroll_straighten_config_t dragscroll_straighten_config;
    dragscroll_straighten_config_t dragscroll_straighten_default_config = {
        #ifdef DRAGSCROLL_STRAIGHTEN_SENSITIVITY
            .state          = true,
            .sensitivity    = DRAGSCROLL_STRAIGHTEN_SENSITIVITY,
        #else // DRAGSCROLL_STRAIGHTEN_SENSITIVITY
            .state          = false,
            .sensitivity    = 80,
        #endif // DRAGSCROLL_STRAIGHTEN_SENSITIVITY
    };

    _Static_assert(sizeof(dragscroll_straighten_config_t) <= EECONFIG_MODULE_DRAGSCROLL_STRAIGHTEN_DATA_SIZE, "EECONFIG_MODULE_DRAGSCROLL_STRAIGHTEN_DATA_SIZE is too small");

    void eeconfig_read_dragscroll_straighten(dragscroll_straighten_config_t *value) {
        eeconfig_read_dragscroll_straighten_datablock(value, 0, sizeof(dragscroll_straighten_config_t));
    }

    void eeconfig_update_dragscroll_straighten(dragscroll_straighten_config_t *value) {
        eeconfig_update_dragscroll_straighten_datablock(value, 0, sizeof(dragscroll_straighten_config_t));
    }

    EECONFIG_DEBOUNCE_HELPER(dragscroll_straighten, dragscroll_straighten_config);

    void keyboard_post_init_dragscroll_straighten(void) {
        eeconfig_init_dragscroll_straighten();
    }

    void eeconfig_init_dragscroll_straighten_datablock(void) {
        dragscroll_straighten_config = dragscroll_straighten_default_config;
        eeconfig_flush_dragscroll_straighten(true);
    }

    void housekeeping_task_dragscroll_straighten(void) {
        eeconfig_flush_dragscroll_straighten_task(1000);
    }
#endif

int8_t history_x[SCROLL_HISTORY_SIZE];
int8_t history_y[SCROLL_HISTORY_SIZE];
uint16_t history_time[SCROLL_HISTORY_SIZE];
uint8_t history_head;
uint8_t history_tail;
bool drgstraight_cancel_x;
bool drgstraight_cancel_y;

bool drgstraight_get_state(void){
    #if defined(DRAGSCROLL_STRAIGHTEN_NOEEPROM)
        return ds_str_state;
    #else
        return dragscroll_straighten_config.state;
    #endif
}

void drgstraight_set_state(bool newstate){
    #if defined(DRAGSCROLL_STRAIGHTEN_NOEEPROM)
        ds_str_state = newstate;
    #else
        dragscroll_straighten_config.state = newstate;
        eeconfig_flag_dragscroll_straighten(true);
    #endif
}

uint8_t drgstraight_get_sensitivity(void){
    #if defined(DRAGSCROLL_STRAIGHTEN_NOEEPROM)
        return sensitivity;
    #else
        return dragscroll_straighten_config.sensitivity;
    #endif
}

void drgstraight_set_sensitivity(uint8_t value){
    if(value > 100){ value = 100; }
    #if defined(DRAGSCROLL_STRAIGHTEN_NOEEPROM)
        sensitivity = value;
    #else
        dragscroll_straighten_config.sensitivity = value;
        eeconfig_flag_dragscroll_straighten(true);
    #endif
}

void drgstraight_reset( void ){
    history_tail = history_head;
    history_x[history_tail] = 0;
    history_y[history_tail] = 0;
}

report_mouse_t pointing_device_task_dragscroll_straighten(report_mouse_t mouse_report) {
    /* Borrowed and adapted from Obosob
    *  https://github.com/obosob/qmk_firmware/blob/2b1d6e6c31ac3ddf1e023143d46acafaac1103e5/keyboards/ploopyco/madromys/keymaps/obosob/keymap.c#L84
    *
    *  Obosob's could negates horizontal scroll when the prevailing scroll direction is vertical.
    *  This code also negates vertical scroll when the prevailing scroll direction is horizontal.
    *  Additionally, the threshold for negating the lesser scroll direction is tuneable.
    */

    // When sampling frequency elapsed
    drgstraight_cancel_x = false;
    drgstraight_cancel_y = false;
    if ( !(drgstraight_get_state() && drgstraight_get_sensitivity()) ){ return mouse_report; }
    if (timer_elapsed(history_time[history_head]) > SCROLL_HISTORY_FREQ) {
        //advance the head of the buffer.
        history_head = (history_head + 1) % SCROLL_HISTORY_SIZE;
        // if head has met the tail, advance the tail by one
        if(history_head == history_tail) {
            history_tail = (history_tail + 1) % SCROLL_HISTORY_SIZE;
        }
        // Start timer and initialise new frame to zero.
        history_time[history_head] = timer_read();
        history_x[history_head] = 0;
        history_y[history_head] = 0;
    }

    // Add mouse report to sample.
    history_x[history_head] += mouse_report.x;
    history_y[history_head] += mouse_report.y;

    // iterate over the history buffer, calculate the velocity for each time
    // step (average velocity for the sample frequency)
    float momentum_x = 0.0;
    float momentum_y = 0.0;
    int8_t i = history_tail;
    while ( (i + 1) % SCROLL_HISTORY_SIZE != (history_head + 1) % SCROLL_HISTORY_SIZE ) {
        momentum_x += (float)abs(history_x[(i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(history_time[(i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(history_time[i]));
        momentum_y += (float)abs(history_y[(i + 1) % SCROLL_HISTORY_SIZE]) / (float)abs(timer_elapsed(history_time[(i + 1) % SCROLL_HISTORY_SIZE]) - timer_elapsed(history_time[i]));
        i = (i + 1) % SCROLL_HISTORY_SIZE;
    }

    // If [sensitivity %] of VERTICAL momentum exceeds HORIZONTAL momentum
    if ( ((float)drgstraight_get_sensitivity() / (float)100) * momentum_y > momentum_x ){
        // Clear HORIZONTAL accumulation
        drgstraight_cancel_x = true;
        // dprintf("cleared horizontal accumulation \n");
    }
    // If [sensitivity %] of HORIZONTAL momentum exceeds VERTICAL momentum
    else if ( ((float)drgstraight_get_sensitivity() / (float)100) * momentum_x > momentum_y ){
        // Clear VERTICAL accumulation where [sensitivity %] of HORIZONTAL momentum exceeds VERTICAL momentum
        drgstraight_cancel_y = true;
        // dprintf("cleared vertical accumulation \n");
    }
    return mouse_report;
}
