#ifndef UI_H
#define UI_H

#include <stdint.h>
#include "../Time/Time.h"

typedef enum {
    UI_NORMAL,
    UI_SHOW_THRESHOLDS,
    UI_SHOW_RECORDS_TEMP,
    UI_SHOW_RECORDS_LUM,
    UI_CONFIG_CLKH,
    UI_CONFIG_CLKM,
    UI_CONFIG_CLKS,
    UI_CONFIG_C,
    UI_CONFIG_C_ALAH,
    UI_CONFIG_C_ALAM,
    UI_CONFIG_C_ALAS,
    UI_CONFIG_T,
    UI_CONFIG_T_ALAT,
    UI_CONFIG_L,
    UI_CONFIG_L_ALAL,
    UI_CONFIG_A_ALAF,
    UI_RECORDS_RESET
} UI_State;

extern UI_State state;

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
    ui_record_t minimum_temperature;
    ui_record_t maximum_luminosity;
    ui_record_t minimum_luminosity;
} ui_display_data_t;

extern volatile uint8_t ui_timeout_pending;

void ui_init(void);
void ui_update(ui_display_data_t *data, uint8_t s1_pressed,
               uint8_t s2_pressed);


#endif