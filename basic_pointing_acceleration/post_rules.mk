ifeq ($(strip $(VIA_ENABLE)), yes)
    ifeq ($(strip $(POINTING_DEVICE_ACCEL_VIA_ENABLE)), yes)
        OPT_DEFS += -DPOINTING_DEVICE_ACCEL_VIA_ENABLE
        SRC += basic_pointing_acceleration_via.c
    endif
endif
