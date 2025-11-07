#include "comboflow.h"
#include "deferred_exec.h"
#include "process_combo.h"

#if defined(COMBOFLOW_DEBUG)
    #include "debug.h"
#endif // defined(COMBOFLOW_DEBUG)

uint32_t comboflow_deferred_enable(uint32_t trigger_time, void *cb_arg) {
    if(!is_combo_enabled()) {
        combo_flow_token = INVALID_DEFERRED_TOKEN;
        combo_enable();
        #if defined(COMBOFLOW_DEBUG)
            dprintf("COMBOFLOW re-enable\n");
        #endif // defined(COMBOFLOW_DEBUG)
    }
    else{
        #if defined(COMBOFLOW_DEBUG)
            dprintf("COMBOFLOW re-en...FAIL\n");
        #endif // defined(COMBOFLOW_DEBUG)
    }
    return 0;
}

bool process_record_comboflow(uint16_t keycode, keyrecord_t *record) {
    if(IS_COMBOEVENT(record->event)){
        #if defined(COMBOFLOW_DEBUG)
            dprintf("COMBOFLOW leave it");
        #endif // defined(COMBOFLOW_DEBUG)
        if(combo_flow_token != INVALID_DEFERRED_TOKEN){
            cancel_deferred_exec(combo_flow_token);
        }
        if(! is_combo_enabled()){
            combo_enable();
        }
    }
    else if(is_combo_enabled()) {
        combo_disable();
        #if defined(COMBOFLOW_DEBUG)
            dprintf("COMBOFLOW disable\n");
        #endif // defined(COMBOFLOW_DEBUG)
        combo_flow_token = defer_exec(COMBOFLOW_TERM, comboflow_deferred_enable, NULL);
    }
    else if(combo_flow_token != INVALID_DEFERRED_TOKEN){
        #if defined(COMBOFLOW_DEBUG)
            dprintf("COMBOFLOW EXTEND\n");
        #endif // defined(COMBOFLOW_DEBUG)
        extend_deferred_exec(combo_flow_token, COMBOFLOW_TERM);
    }
    return true;
}
