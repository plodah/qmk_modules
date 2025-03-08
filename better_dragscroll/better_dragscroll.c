#include <stdbool.h>
#include "quantum.h"
#include "better_dragscroll.h"

void dragscroll_toggle(bool pressed){
    if(pressed){
        dragscroll_enabled ^= 1;
    }
}

void dragscroll_momentary(bool pressed){
    dragscroll_enabled = pressed;
}

bool process_record_better_dragscroll(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_better_dragscroll_kb(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case COMMUNITY_MODULE_DRAG_SCROLL_MOMENTARY:
            dragscroll_momentary(record->event.pressed);
            return false;
        case COMMUNITY_MODULE_DRAG_SCROLL_TOGGLE:
            dragscroll_toggle(record->event.pressed);
            return false;
        #if !defined(DRAGSCROLL_INDEFINITE)
        default:
            dragscroll_enabled = 0;
        #endif // !defined(DRAGSCROLL_INDEFINITE)
    }
    return true;
}

report_mouse_t pointing_device_task_better_dragscroll(report_mouse_t mouse_report) {
    if (dragscroll_enabled) {
        dragscroll_acc_h += (float)mouse_report.x / DRAGSCROLL_DIVISOR_H;
        dragscroll_acc_v += (float)mouse_report.y / DRAGSCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report

        #ifdef DRAGSCROLL_INVERT_H
          mouse_report.h = -(int8_t)dragscroll_acc_h;
        #else
          mouse_report.h = (int8_t)dragscroll_acc_h;
        #endif // DRAGSCROLL_INVERT_V
        #ifdef DRAGSCROLL_INVERT_V
          mouse_report.v = -(int8_t)dragscroll_acc_v;
        #else
          mouse_report.v = (int8_t)dragscroll_acc_v;
        #endif // DRAGSCROLL_INVERT_V

        // Update accumulated scroll values by subtracting the integer parts
        dragscroll_acc_h -= (int8_t)dragscroll_acc_h;
        dragscroll_acc_v -= (int8_t)dragscroll_acc_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

#if defined(DRAGSCROLL_SCRLK_ENABLE) || defined(DRAGSCROLL_CAPLK_ENABLE)
  bool led_update_better_dragscroll(led_t led_state) {
    dprintf("scr:%d", led_state.scroll_lock);
    dragscroll_enabled =
    #if defined(DRAGSCROLL_SCRLK_ENABLE)
      led_state.scroll_lock
      #if defined(DRAGSCROLL_CAPLK_ENABLE)
        ||
      #endif
    #endif // DRAGSCROLL_SCRLK_ENABLE

    #if defined(DRAGSCROLL_CAPLK_ENABLE)
      led_state.caps_lock
    #endif // DRAGSCROLL_SCRLK_ENABLE
    ;
    return true;
  }
#endif // defined(DRAGSCROLL_SCRLK_ENABLE) || defined(DRAGSCROLL_CAPLK_ENABLE)
