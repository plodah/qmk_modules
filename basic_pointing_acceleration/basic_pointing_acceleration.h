// Copyright 2024 burkfers (@burkfers)
// Copyright 2024 Wimads (@wimads)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "action.h"
#include "report.h"

// Controls the gradient of the acceleration slope. Higher = faster growth.
#ifndef POINTING_DEVICE_ACCEL_GROWTH_RATE
#    define POINTING_DEVICE_ACCEL_GROWTH_RATE 0.25
#endif // POINTING_DEVICE_ACCEL_GROWTH_RATE
// lower/higher value = acceleration kicks in earlier/later
#ifndef POINTING_DEVICE_ACCEL_OFFSET
#    define POINTING_DEVICE_ACCEL_OFFSET 2.2
#endif // POINTING_DEVICE_ACCEL_OFFSET
// lower limit of accel curve (minimum acceleration factor)
#ifndef POINTING_DEVICE_ACCEL_LIMIT
#    define POINTING_DEVICE_ACCEL_LIMIT 0.2
#endif // POINTING_DEVICE_ACCEL_LIMIT
// milliseconds to wait between requesting the device's current DPI
#ifndef POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS
#    define POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS 200
#endif // POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS
// upper limit of accel curve, recommended to leave at 1; adjust DPI setting instead.
#ifndef POINTING_DEVICE_ACCEL_LIMIT_UPPER
#    define POINTING_DEVICE_ACCEL_LIMIT_UPPER 1
#endif // POINTING_DEVICE_ACCEL_LIMIT_UPPER
// milliseconds after which to reset quantization error correction (forget rounding remainder)
#ifndef POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS
#    define POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS 200
#endif // POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS

#ifndef POINTING_DEVICE_ACCEL_TAKEOFF_STEP
#    define POINTING_DEVICE_ACCEL_TAKEOFF_STEP 0.01f
#endif
#ifndef POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP
#    define POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP 0.01f
#endif
#ifndef POINTING_DEVICE_ACCEL_OFFSET_STEP
#    define POINTING_DEVICE_ACCEL_OFFSET_STEP 0.1f
#endif
#ifndef POINTING_DEVICE_ACCEL_LIMIT_STEP
#    define POINTING_DEVICE_ACCEL_LIMIT_STEP 0.01f
#endif

typedef struct basic_pointing_acceleration_config_t {
    bool  enabled : 1;
    float growth_rate;
    float offset;
    float limit;
    float takeoff;
} basic_pointing_acceleration_config_t;

extern basic_pointing_acceleration_config_t g_basic_pointing_acceleration_config;

report_mouse_t pointing_device_task_basic_pointing_acceleration(report_mouse_t mouse_report);

float basic_pointing_acceleration_get_mod_step(float step);

void basic_pointing_acceleration_enabled(bool enable);
bool basic_pointing_acceleration_get_enabled(void);
void basic_pointing_acceleration_toggle_enabled(void);

float basic_pointing_acceleration_get_takeoff(void);
void  basic_pointing_acceleration_set_takeoff(float val);
void  basic_pointing_acceleration_takeoff_increment(void);

float basic_pointing_acceleration_get_growth_rate(void);
void  basic_pointing_acceleration_set_growth_rate(float val);
void  basic_pointing_acceleration_growth_rate_increment(void);

float basic_pointing_acceleration_get_offset(void);
void  basic_pointing_acceleration_set_offset(float val);
void  basic_pointing_acceleration_offset_increment(void);

float basic_pointing_acceleration_get_limit(void);
void  basic_pointing_acceleration_set_limit(float val);
void  basic_pointing_acceleration_set_limit_increment(void);

void pointing_device_config_update(basic_pointing_acceleration_config_t *config);
void pointing_device_config_read(basic_pointing_acceleration_config_t *config);

void basic_pointing_acceleration_plot_curve(uint8_t graph[], uint8_t graph_size);

bool basic_pointing_acceleration_should_process(void);
