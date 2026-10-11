#include <stdint.h>
#include <stdio.h>
#include "../LCD/lcd.h"
#include "ui.h"

#define UI_REFRESH_MS 250u
#define UI_SCREEN_MS 1000u
#define UI_THRESHOLDS_MS 2000u

UI_State state = UI_NORMAL;
static uint32_t state_started_ms;
static uint32_t last_draw_ms;
static uint8_t redraw = 1u;


static void ui_write_line(uint8_t row, const char *text)
{
    char line[17];

    sprintf(line, "%-16.16s", text);
    LCDpos(row, 0);
    LCDstr(line);
}

static void ui_write_temperature(int8_t temperature)
{
    char text[6];

    sprintf(text, "%dC", (int)temperature);
    LCDstr(text);
}

static void ui_write_luminosity(uint8_t luminosity)
{
    char text[4];

    sprintf(text, "%u", (unsigned int)luminosity);
    LCDstr(text);
}

static void ui_write_HH(uint8_t hours)
{
    char text[3];

    sprintf(text, "%02u", (unsigned int)hours);
    LCDstr(text);
}

static void ui_write_mm(uint8_t minutes)
{
    char text[3];

    sprintf(text, "%02u", (unsigned int)minutes);
    LCDstr(text);
}

static void ui_write_ss(uint8_t seconds)
{
    char text[3];

    sprintf(text, "%02u", (unsigned int)seconds);
    LCDstr(text);
}

static void ui_write_clock(const time_t *time)
{
    ui_write_HH(time->hour);
    LCDstr(":");
    ui_write_mm(time->minute);
    LCDstr(":");
    ui_write_ss(time->second);
}

static void ui_show_cursor(uint8_t row, uint8_t column)
{
    LCDpos(row, column);
    LCDcmd(0x0f);
}

static void ui_hide_cursor(void)
{
    LCDcmd(0x0c);
}

static void ui_enter(UI_State next, uint32_t now_ms)
{
    state = next;
    state_started_ms = now_ms;
    redraw = 1u;
}

static void ui_draw_normal(const ui_display_data_t *data)
{
    char line[17];

    LCDpos(0, 0);
    ui_write_clock(&data->current_time);
    sprintf(line, " %c%c%c%c",
            (data->alarm_notifications & UI_ALARM_CLOCK) ? 'C' : ' ',
            (data->alarm_notifications & UI_ALARM_TEMP) ? 'T' : ' ',
            (data->alarm_notifications & UI_ALARM_LUM) ? 'L' : ' ',
            data->alarms_enabled ? 'A' : 'a');
    LCDstr(line);

    ui_write_line(1, "T:");
    LCDpos(1, 2);
    ui_write_temperature(data->temperature);
    LCDstr(" L:");
    ui_write_luminosity(data->luminosity);
    ui_hide_cursor();
}

static void ui_draw_thresholds(const ui_display_data_t *data)
{
    ui_write_line(0, "Alarm ");
    LCDpos(0, 6);
    ui_write_HH(data->alarm_time.hour);
    LCDstr(":");
    ui_write_mm(data->alarm_time.minute);

    ui_write_line(1, "T:");
    LCDpos(1, 2);
    ui_write_temperature((int8_t)data->temperature_threshold);
    LCDstr("  L:");
    ui_write_luminosity(data->luminosity_threshold);
    ui_hide_cursor();
}

static void ui_draw_config(const ui_display_data_t *data)
{
    char line[17];
    uint8_t row = 0u;
    uint8_t column = 0u;

    if (state == UI_CONFIG_CLKH || state == UI_CONFIG_CLKM ||
        state == UI_CONFIG_CLKS) {
        ui_write_line(0, "Clock ");
        LCDpos(0, 6);
        ui_write_clock(&data->current_time);

        if (state == UI_CONFIG_CLKH)
            column = 6u;
        else if (state == UI_CONFIG_CLKM)
            column = 9u;
        else
            column = 12u;
    } else if (state == UI_CONFIG_C_ALAH || state == UI_CONFIG_C_ALAM || 
        state == UI_CONFIG_C_ALAS) {
        ui_write_line(0, "Alarm ");
        LCDpos(0, 6);
        ui_write_clock(&data->alarm_time);

        if (state == UI_CONFIG_C_ALAH)
            column = 6u;
        else if (state == UI_CONFIG_C_ALAM)
            column = 9u;
        else
            column = 12u;
    } else if (state == UI_CONFIG_T || state == UI_CONFIG_T_ALAT) {
        ui_write_line(0, "Temperature");
        ui_write_line(1, "Threshold: ");
        LCDpos(1, 11);
        ui_write_temperature((int8_t)data->temperature_threshold);
        row = 1u;
        column = 11u;
    } else if (state == UI_CONFIG_L || state == UI_CONFIG_L_ALAL) {
        ui_write_line(0, "Luminosity");
        ui_write_line(1, "Threshold: ");
        LCDpos(1, 11);
        ui_write_luminosity(data->luminosity_threshold);
        row = 1u;
        column = 11u;
    } else if (state == UI_CONFIG_A_ALAF) {
        ui_write_line(0, data->alarms_enabled
                      ? "Alarm: enabled" : "Alarm: disabled");
        ui_write_line(1, "S1 next  S2 tog");
        row = 1u;
        column = 12u;
    } else {
        ui_write_line(0, "Reset records?");
        ui_write_line(1, "S1 exit  S2 yes");
        row = 1u;
        column = 0u;
    }

    ui_show_cursor(row, column);
}

static void ui_draw_temperature_records(const ui_display_data_t *data)
{
    char line[17];

    ui_write_line(0, "Max ");
    LCDpos(0, 4);
    ui_write_temperature(data->maximum_temperature.temperature);
    sprintf(line, " %02u:%02u",
            (unsigned int)data->maximum_temperature.timestamp.hour,
            (unsigned int)data->maximum_temperature.timestamp.minute);
    LCDstr(line);

    ui_write_line(1, "Min ");
    LCDpos(1, 4);
    ui_write_temperature(data->minimum_temperature.temperature);
    sprintf(line, " %02u:%02u",
            (unsigned int)data->minimum_temperature.timestamp.hour,
            (unsigned int)data->minimum_temperature.timestamp.minute);
    LCDstr(line);
    ui_hide_cursor();
}

static void ui_draw_luminosity_records(const ui_display_data_t *data)
{
    char line[17];

    ui_write_line(0, "Max L");
    LCDpos(0, 5);
    ui_write_luminosity(data->maximum_luminosity.luminosity);
    sprintf(line, " %02u:%02u",
            (unsigned int)data->maximum_luminosity.timestamp.hour,
            (unsigned int)data->maximum_luminosity.timestamp.minute);
    LCDstr(line);

    ui_write_line(1, "Min L");
    LCDpos(1, 5);
    ui_write_luminosity(data->minimum_luminosity.luminosity);
    sprintf(line, " %02u:%02u",
            (unsigned int)data->minimum_luminosity.timestamp.hour,
            (unsigned int)data->minimum_luminosity.timestamp.minute);
    LCDstr(line);
    ui_hide_cursor();
}

static void ui_draw(const ui_display_data_t *data)
{
    switch (state) {
        case UI_NORMAL:
            ui_draw_normal(data);
            break;
        case UI_SHOW_THRESHOLDS:
            ui_draw_thresholds(data);
            break;
        case UI_SHOW_RECORDS_TEMP:
            ui_draw_temperature_records(data);
            break;
        case UI_SHOW_RECORDS_LUM:
            ui_draw_luminosity_records(data);
            break;
        default:
            ui_draw_config(data);
            break;
    }
}

static void ui_increment_time_field(time_t *time, uint8_t field)
{
    if (field == 0u)
        time->hour = (uint8_t)((time->hour + 1u) % 24u);
    else if (field == 1u)
        time->minute = (uint8_t)((time->minute + 1u) % 60u);
    else
        time->second = (uint8_t)((time->second + 1u) % 60u);
}

static void ui_update_timed_screen(UI_State next, uint32_t now_ms)
{
    if ((uint32_t)(now_ms - state_started_ms) >= UI_SCREEN_MS)
        ui_enter(next, now_ms);
}

static void ui_update_threshold(uint8_t *threshold, uint8_t maximum,
                                UI_State edit_state, UI_State next_state,
                                uint8_t s1_pressed, uint8_t s2_pressed,
                                uint32_t now_ms)
{
    if (s1_pressed) {
        ui_enter(next_state, now_ms);
    } else if (s2_pressed) {
        if (state == edit_state) {
            *threshold = (uint8_t)((*threshold + 1u) % (maximum + 1u));
            redraw = 1u;
        } else {
            ui_enter(edit_state, now_ms);
        }
    }
}

static void clear_record(ui_record_t *record)
{
    record->timestamp.hour = 0u;
    record->timestamp.minute = 0u;
    record->timestamp.second = 0u;
    record->temperature = 0;
    record->luminosity = 0u;
}

static void ui_reset_records(ui_display_data_t *data)
{
    clear_record(&data->maximum_temperature);
    clear_record(&data->minimum_temperature);
    clear_record(&data->maximum_luminosity);
    clear_record(&data->minimum_luminosity);
}

void ui_update(ui_display_data_t *data, uint8_t s1_pressed,
               uint8_t s2_pressed, uint32_t now_ms)
{
    if (data == NULL)
        return;

    switch (state) {
        case UI_NORMAL:
            if (s1_pressed) {
                data->alarm_notifications = 0u;
                ui_enter(UI_SHOW_THRESHOLDS, now_ms);
            } else if (s2_pressed) {
                ui_enter(UI_SHOW_RECORDS_TEMP, now_ms);
            }
            break;

        case UI_SHOW_THRESHOLDS:
            if (s1_pressed)
                ui_enter(UI_CONFIG_CLKH, now_ms);
            else if ((uint32_t)(now_ms - state_started_ms) >= UI_THRESHOLDS_MS)
                ui_enter(UI_NORMAL, now_ms);
            break;

        case UI_SHOW_RECORDS_TEMP:
        case UI_SHOW_RECORDS_LUM:
            ui_update_timed_screen(
                state == UI_SHOW_RECORDS_TEMP ? UI_SHOW_RECORDS_LUM : UI_NORMAL, now_ms);
            break;

        case UI_CONFIG_CLKH:
        case UI_CONFIG_CLKM:
        case UI_CONFIG_CLKS:
            if (s1_pressed) {
                ui_enter(state + 1, now_ms);
            } else if (s2_pressed) {
                ui_increment_time_field(&data->current_time,
                                        (uint8_t)(state - UI_CONFIG_CLKH));
                redraw = 1u;
            }
            break;

        case UI_CONFIG_C:
            if (s1_pressed)
                ui_enter(UI_CONFIG_T, now_ms);
            else if (s2_pressed) {
                ui_enter(UI_CONFIG_C_ALAH, now_ms);
            }
            break;

        case UI_CONFIG_C_ALAH:
        case UI_CONFIG_C_ALAM:
        case UI_CONFIG_C_ALAS:
            if (s1_pressed) {
                ui_enter(state + 1, now_ms);
            } else if (s2_pressed) {
                ui_increment_time_field(&data->alarm_time, 
                                        (uint8_t)(state - UI_CONFIG_C_ALAH));
                redraw = 1u;
            }
            break;

        case UI_CONFIG_T:
        case UI_CONFIG_T_ALAT:
            ui_update_threshold(&data->temperature_threshold, 50u,
                                UI_CONFIG_T_ALAT, UI_CONFIG_L,
                                s1_pressed, s2_pressed, now_ms);
            break;

        case UI_CONFIG_L:
        case UI_CONFIG_L_ALAL:
            ui_update_threshold(&data->luminosity_threshold, 7u,
                                UI_CONFIG_L_ALAL, UI_CONFIG_A_ALAF,
                                s1_pressed, s2_pressed, now_ms);
            break;

        case UI_CONFIG_A_ALAF:
            if (s1_pressed)
                ui_enter(UI_RECORDS_RESET, now_ms);
            else if (s2_pressed) {
                data->alarms_enabled = !data->alarms_enabled;
                redraw = 1u;
            }
            break;

        case UI_RECORDS_RESET:
            if (s1_pressed)
                ui_enter(UI_NORMAL, now_ms);
            else if (s2_pressed) {
                ui_reset_records(data);
                redraw = 1u;
            }
            break;

        default:
            ui_enter(UI_NORMAL, now_ms);
            break;
    }

    if (redraw ||
        ((state == UI_NORMAL || state == UI_SHOW_THRESHOLDS) &&
         (uint32_t)(now_ms - last_draw_ms) >= UI_REFRESH_MS)) {
        ui_draw(data);
        last_draw_ms = now_ms;
        redraw = 0u;
    }
}
