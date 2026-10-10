#ifndef UI_H
#define UI_H

#include <stdint.h>
#include "Time/Time.h"

typedef enum {
    UI_NORMAL,
    UI_SHOW_THRESHOLDS,
    UI_SHOW_TEMP,
    UI_SHOW_LUM,
    UI_CONFIG_H,
    UI_CONFIG_M,
    UI_CONFIG_S,
    UI_CONFIG_C,
    UI_CONFIG_C_EDIT,
    UI_CONFIG_T,
    UI_CONFIG_T_EDIT,
    UI_CONFIG_L,
    UI_CONFIG_L_EDIT,
    UI_CONFIG_ALARM,
    UI_CONFIG_RESET
} UI_State;

#define UI_ALARM_CLOCK 0x01u
#define UI_ALARM_TEMP  0x02u
#define UI_ALARM_LUM   0x04u

typedef struct {
    time_t timestamp;
    int8_t temperature;
    uint8_t luminosity;
} ui_record_t;

typedef struct {
    time_t current_time;
    time_t alarm_time;
    int8_t temperature;
    uint8_t luminosity;
    uint8_t temperature_threshold;
    uint8_t luminosity_threshold;
    uint8_t alarms_enabled;
    uint8_t alarm_notifications;
    ui_record_t maximum_temperature;
    time_t maximum_temperature_timestamp;
    ui_record_t minimum_temperature;
    time_t minimum_temperature_timestamp;
    ui_record_t maximum_luminosity;
    time_t maximum_luminosity_timestamp;
    ui_record_t minimum_luminosity;
    time_t minimum_luminosity_timestamp;
} ui_display_data_t;

void ui_update(ui_display_data_t *data, uint8_t s1_pressed,
               uint8_t s2_pressed, uint32_t now_ms);

#endif