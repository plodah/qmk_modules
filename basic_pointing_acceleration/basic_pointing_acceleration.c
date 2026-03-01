// Copyright 2021 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2024 burkfers (@burkfers)
// Copyright 2024 Wimads (@wimads)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "pointing_device_internal.h"
#include "basic_pointing_acceleration.h"
#include "math.h"
#include <quantum/util.h>

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 1, 0);

static uint32_t basic_pointing_acceleration_timer;

basic_pointing_acceleration_config_t g_basic_pointing_acceleration_config;

float basic_pointing_acceleration_get_growth_rate(void) {
    return g_basic_pointing_acceleration_config.growth_rate;
}

void basic_pointing_acceleration_set_growth_rate(float val) {
    if (val >= 0) { // value less 0 leads to nonsensical results
        g_basic_pointing_acceleration_config.growth_rate = val;
        pointing_device_config_update(&g_basic_pointing_acceleration_config);
    }
}

float basic_pointing_acceleration_get_offset(void) {
    return g_basic_pointing_acceleration_config.offset;
}

void basic_pointing_acceleration_set_offset(float val) {
    g_basic_pointing_acceleration_config.offset = val;
    pointing_device_config_update(&g_basic_pointing_acceleration_config);
}

float basic_pointing_acceleration_get_limit(void) {
    return g_basic_pointing_acceleration_config.limit;
}

void basic_pointing_acceleration_set_limit(float val) {
    if (val >= 0) {
        g_basic_pointing_acceleration_config.limit = val;
        pointing_device_config_update(&g_basic_pointing_acceleration_config);
    }
}

void basic_pointing_acceleration_enabled(bool enable) {
    g_basic_pointing_acceleration_config.enabled = enable;
    pointing_device_config_update(&g_basic_pointing_acceleration_config);
    pd_dprintf("PDACCEL: enabled: %d\n", g_basic_pointing_acceleration_config.enabled);
}

bool basic_pointing_acceleration_get_enabled(void) {
    return g_basic_pointing_acceleration_config.enabled;
}

void basic_pointing_acceleration_toggle_enabled(void) {
    basic_pointing_acceleration_enabled(!basic_pointing_acceleration_get_enabled());
}

__attribute__((weak)) bool basic_pointing_acceleration_should_process(void) {
    return true;
}

report_mouse_t pointing_device_task_basic_pointing_acceleration(report_mouse_t mouse_report) {
    // rounding carry to recycle dropped floats from int mouse reports, to smoothen low speed movements (credit
    // @ankostis)
    static float rounding_carry_x = 0;
    static float rounding_carry_y = 0;
    // time since last mouse report:
    const uint16_t delta_time = timer_elapsed32(basic_pointing_acceleration_timer);
    // skip basic_pointing_acceleration maths if report = 0, or if basic_pointing_acceleration not enabled.
    if ((mouse_report.x == 0 && mouse_report.y == 0) || !g_basic_pointing_acceleration_config.enabled ||
        !basic_pointing_acceleration_should_process()) {
        return mouse_report;
    }
    // reset timer:
    basic_pointing_acceleration_timer = timer_read32();
    // Reset carry if too much time passed
    if (delta_time > POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS) {
        rounding_carry_x = 0;
        rounding_carry_y = 0;
    }
    // Reset carry when pointer swaps direction, to follow user's hand.
    if (mouse_report.x * rounding_carry_x < 0) rounding_carry_x = 0;
    if (mouse_report.y * rounding_carry_y < 0) rounding_carry_y = 0;
    // Limit expensive calls to get device cpi settings only when mouse stationary for > 200ms.
    static uint16_t device_cpi = 300;
    if (delta_time > POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS) {
        device_cpi = pointing_device_get_cpi();
    }
    // calculate dpi correction factor (for normalizing velocity range across different user dpi settings)
    const float dpi_correction = (float)1000.0f / device_cpi;
    // calculate euclidean distance moved (sqrt(x^2 + y^2))
    const float distance = sqrtf(mouse_report.x * mouse_report.x + mouse_report.y * mouse_report.y);
    // calculate delta velocity: dv = distance/dt
    const float velocity_raw = distance / delta_time;
    // correct raw velocity for dpi
    const float velocity = dpi_correction * velocity_raw;
    // letter variables for readability of maths:
    const float g = g_basic_pointing_acceleration_config.growth_rate;
    const float s = g_basic_pointing_acceleration_config.offset;
    const float m = g_basic_pointing_acceleration_config.limit;
    // acceleration factor: f(v) = 1 - (1 - M) / {1 + e^[K(v - S)]}^(G/K):
    // Generalised Sigmoid Function, see https://www.desmos.com/calculator/k9vr0y2gev
    float basic_pointing_acceleration_factor = (g / 2)*(velocity - s) + m;
    if(basic_pointing_acceleration_factor < m){ basic_pointing_acceleration_factor = m;}
    else if(basic_pointing_acceleration_factor > 1){ basic_pointing_acceleration_factor = 1;}
        //POINTING_DEVICE_ACCEL_LIMIT_UPPER -
        //(POINTING_DEVICE_ACCEL_LIMIT_UPPER - m) / powf(1 + expf(k * (velocity - s)), g / k);
    // multiply mouse reports by acceleration factor, and account for previous quantization errors:
    const float new_x = rounding_carry_x + basic_pointing_acceleration_factor * mouse_report.x;
    const float new_y = rounding_carry_y + basic_pointing_acceleration_factor * mouse_report.y;
    // Accumulate any difference from next integer (quantization).
    rounding_carry_x = new_x - (int)new_x;
    rounding_carry_y = new_y - (int)new_y;
    // clamp values
    const mouse_xy_report_t x = CONSTRAIN_HID_XY(new_x);
    const mouse_xy_report_t y = CONSTRAIN_HID_XY(new_y);

#ifdef POINTING_DEVICE_DEBUG
    // console output for debugging (enable/disable in config.h)
    const float distance_out = sqrtf(x * x + y * y);
    const float velocity_out = velocity * basic_pointing_acceleration_factor;
    (void)distance_out;
    (void)velocity_out;
    pd_dprintf("PDACCEL: DPI:%4i Grw: %.3f Ofs: %.3f Lmt: %.3f | Fct: %.3f v.in: %.3f v.out: %.3f d.in: %3i "
               "d.out: %3i\n",
               device_cpi, g_basic_pointing_acceleration_config.growth_rate,
               g_basic_pointing_acceleration_config.offset, g_basic_pointing_acceleration_config.limit,
               basic_pointing_acceleration_factor, velocity, velocity_out, (mouse_xy_report_t)CONSTRAIN_HID_XY(distance),
               (mouse_xy_report_t)CONSTRAIN_HID_XY(distance_out));
#endif // POINTING_DEVICE_DEBUG
    // report back accelerated values
    mouse_report.x = x;
    mouse_report.y = y;

    return pointing_device_task_basic_pointing_acceleration_kb(mouse_report);
}

float basic_pointing_acceleration_get_mod_step(float step) {
    const uint8_t mod_mask = get_mods();
    if (mod_mask & MOD_MASK_CTRL) {
        step *= 10; // control increases by factor 10
    }
    if (mod_mask & MOD_MASK_SHIFT) {
        step *= -1; // shift inverts
    }
    return step;
}

void basic_pointing_acceleration_growth_rate_increment(void) {
    basic_pointing_acceleration_set_growth_rate(basic_pointing_acceleration_get_growth_rate() +
                                          basic_pointing_acceleration_get_mod_step(POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP));
    pd_dprintf("PDACCEL:keycode: GRO: %.3f ofs: %.3f lmt: %.3f\n",
               g_basic_pointing_acceleration_config.growth_rate, g_basic_pointing_acceleration_config.offset,
               g_basic_pointing_acceleration_config.limit);
}

void basic_pointing_acceleration_offset_increment(void) {
    basic_pointing_acceleration_set_offset(basic_pointing_acceleration_get_offset() +
                                     basic_pointing_acceleration_get_mod_step(POINTING_DEVICE_ACCEL_OFFSET_STEP));
    pd_dprintf("PDACCEL:keycode: gro: %.3f OFS: %.3f lmt: %.3f\n",
               g_basic_pointing_acceleration_config.growth_rate, g_basic_pointing_acceleration_config.offset,
               g_basic_pointing_acceleration_config.limit);
}

void basic_pointing_acceleration_set_limit_increment(void) {
    basic_pointing_acceleration_set_limit(basic_pointing_acceleration_get_limit() +
                                    basic_pointing_acceleration_get_mod_step(POINTING_DEVICE_ACCEL_LIMIT_STEP));
    pd_dprintf("PDACCEL:keycode: gro: %.3f ofs: %.3f LMT: %.3f\n",
               g_basic_pointing_acceleration_config.growth_rate, g_basic_pointing_acceleration_config.offset,
               g_basic_pointing_acceleration_config.limit);
}

bool process_record_basic_pointing_acceleration(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_basic_pointing_acceleration_kb(keycode, record)) {
        return true;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case CM_MOUSE_ACCEL_TOGGLE:
                basic_pointing_acceleration_toggle_enabled();
                break;
            /*
            case CM_MOUSE_ACCEL_GROWTH_RATE:
                basic_pointing_acceleration_growth_rate_increment();
                break;
            case CM_MOUSE_ACCEL_OFFSET:
                basic_pointing_acceleration_offset_increment();
                break;
            case CM_MOUSE_ACCEL_LIMIT:
                basic_pointing_acceleration_set_limit_increment();
                break;
            */
        }
    }
    return true;
}

__attribute__((weak)) void pointing_device_config_update(basic_pointing_acceleration_config_t *config) {
    // Co nothing since we're saving/storing in memory.
    // Can be overridden to implement NVM storage.  VIA module does this, in fact.
}

__attribute__((weak)) void pointing_device_config_read(basic_pointing_acceleration_config_t *config) {
    g_basic_pointing_acceleration_config = (basic_pointing_acceleration_config_t){
        .growth_rate = POINTING_DEVICE_ACCEL_GROWTH_RATE,
        .offset      = POINTING_DEVICE_ACCEL_OFFSET,
        .limit       = POINTING_DEVICE_ACCEL_LIMIT,
        .enabled     = true,
    };
}

__attribute__((weak)) void keyboard_post_init_basic_pointing_acceleration(void) {
    // Read initial config into memory.
    // Via module reads settings from NVM into memory.
    pointing_device_config_read(&g_basic_pointing_acceleration_config);

    keyboard_post_init_basic_pointing_acceleration_kb();
}

// Unfortunately, there is no way to automatically call this as of 1.0.0 or 1.1.0
// Compounding this is that NVM abstraction prevents us from saving to arbitrary locations, safely.
// We could still do so, but to future proof this .... for now, it needs to be called from either
// eeconfig_init_user(void) or eeconfig_init_kb(void).
void eeconfig_init_pointing_device(void) {
    g_basic_pointing_acceleration_config = (basic_pointing_acceleration_config_t){
        .growth_rate = POINTING_DEVICE_ACCEL_GROWTH_RATE,
        .offset      = POINTING_DEVICE_ACCEL_OFFSET,
        .limit       = POINTING_DEVICE_ACCEL_LIMIT,
        .enabled     = true,
    };
    // Write default value to EEPROM now
    pointing_device_config_update(&g_basic_pointing_acceleration_config);
}

/**
 * @brief Plots an acceleration curve for pointing device acceleration
 *
 * This function generates a acceleration curve and stores the computed values in the provided graph array. The curve
 * is calculated using the current acceleration configuration parameters and follows a logistic growth function.
 *
 * @param graph      Array to store the computed acceleration curve values (0-100 scale)
 * @param graph_size Size of the graph array, determines the velocity range (0 to graph_size-1)
 *
 * @note The function uses global acceleration configuration from g_basic_pointing_acceleration_config
 * @note Values are scaled by 100 and cast to uint8_t for storage
 * @note Function isn't very scalable.

void basic_pointing_acceleration_plot_curve(uint8_t graph[], uint8_t graph_size) {
    const float g = g_basic_pointing_acceleration_config.growth_rate;
    const float s = g_basic_pointing_acceleration_config.offset;
    const float m = g_basic_pointing_acceleration_config.limit;

    for (uint8_t velocity = 0; velocity < graph_size; velocity++) {
        graph[velocity] =

            (uint8_t)((POINTING_DEVICE_ACCEL_LIMIT_UPPER -
                       (POINTING_DEVICE_ACCEL_LIMIT_UPPER - m) / powf(1 + expf(k * (velocity - s)), g / k)) *
                      100.0f);
        pd_dprintf("PDACCEL: velocity: %3i, factor: %3d\n", velocity, graph[velocity]);
    }
}
 */
