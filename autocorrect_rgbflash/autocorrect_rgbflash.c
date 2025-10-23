#include "autocorrect_rgbflash.h"

uint8_t autocorrect_rgbflash_count = 255;

#if defined(AUTOCORRECT_RGBFLASH_MODE_DE)
// #pragma message("Deferred Exec Autocorrect")
uint32_t autocorrect_rgbflash_switch_on_DE(uint32_t trigger_time, void *cb_arg) {
  autocorrect_rgbflash_switch_on();
  return 0;
}
uint32_t autocorrect_rgbflash_off_DE(uint32_t trigger_time, void *cb_arg) {
  autocorrect_rgbflash_switch_off();
  return 0;
}
deferred_token autoCorrectIndToken = INVALID_DEFERRED_TOKEN;

#else // AUTOCORRECT_RGBFLASH_MODE_DE
    bool autocorrect_rgbflash_on = false;
    uint16_t autocorrect_rgbflash_timer = 0;
#endif

void autocorrect_rgbflash_switch_on(void) {
#if defined(RGB_MATRIX_ENABLE)
  HSV indhsv = rgbhelpers_limit_ind(rgb_matrix_get_hsv(), AUTOCORRECT_RGBFLASH_COLOR, AUTOCORRECT_RGBFLASH_MINVAL);
  rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  rgb_matrix_sethsv_noeeprom(indhsv.h, indhsv.s, indhsv.v);
#endif // RGB_MATRIX_ENABLE
#if defined(RGBLIGHT_ENABLE)
  HSV indhsv = {rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val()};
  indhsv = rgbhelpers_limit_ind(indhsv, AUTOCORRECT_RGBFLASH_COLOR, AUTOCORRECT_RGBFLASH_MINVAL);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  rgblight_sethsv_noeeprom(indhsv.h, indhsv.s, indhsv.v);
#endif // RGBLIGHT_ENABLE

#if defined(AUTOCORRECT_RGBFLASH_MODE_DE)
  autoCorrectIndToken = defer_exec(AUTOCORRECT_RGBFLASH_DURATION, autocorrect_rgbflash_off_DE, NULL);
#else  // AUTOCORRECT_RGBFLASH_MODE_DE
  autocorrect_rgbflash_on = true;
  autocorrect_rgbflash_timer = timer_read();
#endif // AUTOCORRECT_RGBFLASH_MODE_DE
}

void autocorrect_rgbflash_switch_off(void) {
#if defined(RGB_MATRIX_ENABLE)
  rgb_matrix_reload_from_eeprom();
#endif // RGB_MATRIX_ENABLE
#if defined(RGBLIGHT_ENABLE)
  rgblight_reload_from_eeprom();
#endif // RGBLIGHT_ENABLE
  autocorrect_rgbflash_count++;
#if defined(AUTOCORRECT_RGBFLASH_MODE_DE)
  if (autocorrect_rgbflash_count < AUTOCORRECT_RGBFLASH_BLINKCOUNT) {
    autoCorrectIndToken = defer_exec(AUTOCORRECT_RGBFLASH_DURATION, autocorrect_rgbflash_switch_on_DE, NULL);
  } else {
    autoCorrectIndToken = INVALID_DEFERRED_TOKEN;
  }
#else  // AUTOCORRECT_RGBFLASH_MODE_DE
  autocorrect_rgbflash_on = false;
  autocorrect_rgbflash_timer = timer_read();
#endif // AUTOCORRECT_RGBFLASH_MODE_DE
}

#if !defined(AUTOCORRECT_RGBFLASH_MODE_DE)
  void housekeeping_task_autocorrect_rgbflash(void) {
  if (autocorrect_rgbflash_count < AUTOCORRECT_RGBFLASH_BLINKCOUNT) {
      if (timer_elapsed(autocorrect_rgbflash_timer) >
          AUTOCORRECT_RGBFLASH_DURATION) {
      if (autocorrect_rgbflash_on) {
          autocorrect_rgbflash_switch_off();
      } else {
          autocorrect_rgbflash_switch_on();
      }
      }
  }
  }
#endif // ! AUTOCORRECT_RGBFLASH_MODE_DE

bool apply_autocorrect_autocorrect_rgbflash(uint8_t backspaces, const char *str, char *typo, char *correct) {
  autocorrect_rgbflash_count = 0;
  #if defined(AUTOCORRECT_RGBFLASH_MODE_DE)
      autoCorrectIndToken = defer_exec(AUTOCORRECT_RGBFLASH_DURATION, autocorrect_rgbflash_switch_on_DE, NULL);
  #endif // defined(AUTOCORRECT_RGBFLASH_MODE_DE)
  return true;
}
