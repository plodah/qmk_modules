#pragma once
#include "action.h"
#include "deferred_exec.h"

#if !defined(COMBOFLOW_TERM)
    #define COMBOFLOW_TERM COMBO_TERM
#endif // COMBOFLOW_TERM

deferred_token combo_flow_token = INVALID_DEFERRED_TOKEN;
bool process_record_comboflow(uint16_t keycode, keyrecord_t *record);
