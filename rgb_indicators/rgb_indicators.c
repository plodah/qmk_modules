#include QMK_KEYBOARD_H
#include "rgb_indicators.h"

#if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)
  #include "mouse_jiggler.h"
#endif // defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE)

#if defined(PLODAH_RGBINDICATORS_TYPING_INDEX)
  bool process_record_rgb_indicators(uint16_t keycode, keyrecord_t *record){
    // Some madness to include layer taps / mod taps
    // QK_MOD_TAP        = 0x2000,
    // QK_MOD_TAP_MAX    = 0x3FFF,
    // QK_LAYER_TAP      = 0x4000,
    // QK_LAYER_TAP_MAX  = 0x4FFF,
    if(
        record->event.pressed && (keycode & 0x00FF) >= KC_A && (keycode & 0x00FF) <= KC_SLASH &&
        ( IS_QK_BASIC(keycode) || IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode) )
    ) {
      plodah_rgbindicators_typingindicator_timer = timer_read();
      plodah_rgbindicators_typingindicator_active = true;
    }
    return true;
  }

  void housekeeping_task_rgb_indicators(void){
    if(plodah_rgbindicators_typingindicator_active){
      if(timer_elapsed(plodah_rgbindicators_typingindicator_timer) > PLODAH_RGBINDICATORS_TYPING_DURATION){
        plodah_rgbindicators_typingindicator_active = false;
      }
    }
  }
#endif // PLODAH_RGBINDICATORS_TYPING_INDEX

bool rgb_matrix_indicators_rgb_indicators(void) {
  #if defined(PLODAH_RGBINDICATORS_CAPS_INDEX)
    if (
      #if defined PLODAH_RGBINDICATORS_CAPS_INVERT
      !
      #endif
      host_keyboard_led_state().caps_lock
    ){
      RGB capsrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_CAPS_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_CAPS_INDEX, capsrgb.r, capsrgb.g, capsrgb.b);
    }
    else {
      if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_CAPS_INDEX, RGB_OFF);
      }
    }
  #endif // PLODAH_RGBINDICATORS_CAPS_INDEX

  #if defined(PLODAH_RGBINDICATORS_NUM_INDEX)
    if (
      #if defined PLODAH_RGBINDICATORS_NUM_INVERT
      !
      #endif
      host_keyboard_led_state().num_lock
    ){
      RGB numlrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_NUM_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_NUM_INDEX, numlrgb.r, numlrgb.g, numlrgb.b);
    }
    else {
      if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_NUM_INDEX, RGB_OFF);
      }
    }
  #endif // PLODAH_RGBINDICATORS_NUM_INDEX

  #if defined(PLODAH_RGBINDICATORS_SCRL_INDEX)
    if (
      #if defined PLODAH_RGBINDICATORS_SCRLK_INVERT
      !
      #endif
      host_keyboard_led_state().scroll_lock
    ){
      RGB scrlrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_SCRL_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_SCRL_INDEX, scrlrgb.r, scrlrgb.g, scrlrgb.b);
    }
    else {
      if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_SCRL_INDEX, RGB_OFF);
      }
    }
  #endif // PLODAH_RGBINDICATORS_SCRL_INDEX

  #if defined(PLODAH_RGBINDICATORS_LAYER_INDEX1) || defined(PLODAH_RGBINDICATORS_LAYER_INDEXES)
    #if defined(PLODAH_RGBINDICATORS_LAYER_INDEXES)
      uint8_t layer_ind[] = PLODAH_RGBINDICATORS_LAYER_INDEXES;
    #endif // PLODAH_RGBINDICATORS_LAYER_INDEXES

    RGB layrrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_LAYER_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
    #if defined(PLODAH_RGBINDICATORS_LAYER_BG_HSV)
      laybrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_LAYER_BG_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
    #endif // PLODAH_RGBINDICATORS_LAYER_BG_HSV

    int highlayer = get_highest_layer(layer_state | default_layer_state);
    for (int lindex = 0; lindex < DYNAMIC_KEYMAP_LAYER_COUNT; lindex++) {
      #if defined(PLODAH_RGBINDICATORS_LAYER_INDEXES)
        int thisindex = layer_ind[lindex];
      #else // PLODAH_RGBINDICATORS_LAYER_INDEXES
        int thisindex = (lindex + PLODAH_RGBINDICATORS_LAYER_INDEX1);
      #endif // PLODAH_RGBINDICATORS_LAYER_INDEXES
      if (!rgb_matrix_get_flags()) {
        rgb_matrix_set_color(thisindex, RGB_OFF);
      }
      if (highlayer == lindex && lindex >= PLODAH_RGBINDICATORS_LAYER_STARTING) {
        rgb_matrix_set_color(thisindex, layrrgb.r, layrrgb.g, layrrgb.b);
      }
      #if defined(PLODAH_RGBINDICATORS_LAYER_BG_HSV)
        else {
          if (highlayer != lindex && lindex >= PLODAH_RGBINDICATORS_LAYER_STARTING) {
            rgb_matrix_set_color(thisindex, laybrgb.r, laybrgb.g, laybrgb.b);
          }
        }
      #endif // PLODAH_RGBINDICATORS_LAYER_BG_HSV
    }
  #endif // PLODAH_RGBINDICATORS_LAYER_INDEX1 || PLODAH_RGBINDICATORS_LAYER_INDEXES

  #if defined(DYNAMIC_MACRO_ENABLE) && defined(PLODAH_RGBINDICATORS_DYNMACRO_INDEX)
    RGB dmacrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_DYNMACRO_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
    if (!rgb_matrix_get_flags()) {
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_DYNMACRO_INDEX, RGB_OFF);
    }
    if (is_dynamic_macro_recording) {
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_DYNMACRO_INDEX, dmacrgb.r, dmacrgb.g, dmacrgb.b);
    }
  #endif // DYNAMIC_MACRO_ENABLE && PLODAH_RGBINDICATORS_DYNMACRO_INDEX

  #if defined(PLODAH_RGBINDICATORS_TYPING_INDEX)
    RGB typnrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_TYPING_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
    if(!rgb_matrix_get_flags()){
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_TYPING_INDEX, RGB_OFF);
    }
    if(plodah_rgbindicators_typingindicator_active){
      rgb_matrix_set_color(PLODAH_RGBINDICATORS_TYPING_INDEX, typnrgb.r, typnrgb.g, typnrgb.b);
    }
  #endif // PLODAH_RGBINDICATORS_TYPING_INDEX

  #if defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE) && defined(PLODAH_RGBINDICATORS_MSJIGGLER_INDEX)
      RGB msjigrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_MSJIGGLER_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      if(!rgb_matrix_get_flags()){
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MSJIGGLER_INDEX, RGB_OFF);
      }
      #if !defined(MSJIGGLER_NOINTRO)
          RGB msjigintrorgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_MSJIGGLERINTRO_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      #endif // !defined(MSJIGGLER_NOINTRO)
      switch(jiggler_get_state()){
          #if !defined(MSJIGGLER_NOINTRO)
              case 2:
                  rgb_matrix_set_color(PLODAH_RGBINDICATORS_MSJIGGLER_INDEX, msjigintrorgb.r, msjigintrorgb.g, msjigintrorgb.b);
                  break;
          #endif // !defined(MSJIGGLER_NOINTRO)
          case 1:
              rgb_matrix_set_color(PLODAH_RGBINDICATORS_MSJIGGLER_INDEX, msjigrgb.r, msjigrgb.g, msjigrgb.b);
              break;
      }
  #endif // defined(COMMUNITY_MODULE_MOUSE_JIGGLER_ENABLE) && defined(PLODAH_RGBINDICATORS_MSJIGGLER_INDEX)

  #if ( defined(PLODAH_RGBINDICATORS_MOD_LALT_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RALT_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX) )
    RGB modsrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_MODS_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
  #endif // PLODAH_RGBINDICATORS_MOD_LALT_INDEX

  #if defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX)
    if (!rgb_matrix_get_flags()) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX, RGB_OFF);
      #endif // defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX)
      #if defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX, RGB_OFF);
      #endif // defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX)
    }
    if (get_mods() & MOD_MASK_CTRL) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif // defined(PLODAH_RGBINDICATORS_MOD_LCTL_INDEX)
      #if defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif // defined(PLODAH_RGBINDICATORS_MOD_RCTL_INDEX)
    }
  #endif // PLODAH_RGBINDICATORS_MOD_LCTL_INDEX

  #if defined(PLODAH_RGBINDICATORS_MOD_LALT_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RALT_INDEX)
    if (!rgb_matrix_get_flags()) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LALT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LALT_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_LALT_INDEX
      #if defined(PLODAH_RGBINDICATORS_MOD_RALT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RALT_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_RALT_INDEX
    }
    if (get_mods() & MOD_MASK_ALT) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LALT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LALT_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif  // PLODAH_RGBINDICATORS_MOD_LALT_INDEX
      #if defined(PLODAH_RGBINDICATORS_MOD_RALT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RALT_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif  // PLODAH_RGBINDICATORS_MOD_RALT_INDEX
    }
  #endif // PLODAH_RGBINDICATORS_MOD_LALT_INDEX

  #if defined(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX)
    if (!rgb_matrix_get_flags()) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX
      #if defined(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX
    }
    #if defined(CAPS_WORD_ENABLE)
      if (is_caps_word_on() || get_mods() & MOD_MASK_SHIFT) {
    #else // CAPS_WORD_ENABLE
      if (get_mods() & MOD_MASK_SHIFT) {
    #endif // CAPS_WORD_ENABLE
      #if defined(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif  // PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX
      #if defined(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
      #endif  // PLODAH_RGBINDICATORS_MOD_RSHIFT_INDEX
    }
  #endif // PLODAH_RGBINDICATORS_MOD_LSHIFT_INDEX

  #if (defined(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX) || defined(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX)) && (defined(PLODAH_RGBINDICATORS_GUILOCK_ENABLE) || !defined(PLODAH_MODS_INDIC_GUI_DISABLE) )
    if (!rgb_matrix_get_flags()) {
      #if defined(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_LGUI_INDEX
      #if defined(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX)
        rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX, RGB_OFF);
      #endif  // PLODAH_RGBINDICATORS_MOD_RGUI_INDEX
    }
    #if !defined(PLODAH_MODS_INDIC_GUI_DISABLE)
      if ( get_mods() & MOD_MASK_GUI ) {
        #if defined(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX)
          rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
        #endif  // PLODAH_RGBINDICATORS_MOD_LGUI_INDEX
        #if defined(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX)
          rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX, modsrgb.r, modsrgb.g, modsrgb.b);
        #endif  // PLODAH_RGBINDICATORS_MOD_RGUI_INDEX
      }
    #endif // PLODAH_MODS_INDIC_GUI_DISABLE
    #if defined(PLODAH_RGBINDICATORS_GUILOCK_ENABLE)
      RGB guilockrgb = hsv_to_rgb( rgbhelpers_limit_ind( rgb_matrix_get_hsv(), PLODAH_RGBINDICATORS_GUILOCK_HSV, PLODAH_RGBINDICATORS_MINVAL ) );
      if ( keymap_config.no_gui ) {
        #if defined(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX)
          rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_LGUI_INDEX, guilockrgb.r, guilockrgb.g, guilockrgb.b);
        #endif  // PLODAH_RGBINDICATORS_MOD_LGUI_INDEX
        #if defined(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX)
          rgb_matrix_set_color(PLODAH_RGBINDICATORS_MOD_RGUI_INDEX, guilockrgb.r, guilockrgb.g, guilockrgb.b);
        #endif  // PLODAH_RGBINDICATORS_MOD_RGUI_INDEX
      }
    #endif // defined(PLODAH_RGBINDICATORS_GUILOCK_ENABLE)
  #endif // PLODAH_RGBINDICATORS_MOD_LGUI_INDEX

  return false;
}
