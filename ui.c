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

void ui(void) {
    UI_State ui_state = UI_NORMAL;
    switch (ui_state) {
        case UI_NORMAL:
            if (S1_pressed)
            {
            /*
            * S1:
            *  - limpa as notificações de alarme
            *  - entra na apresentação dos thresholds
            */
                ui_timer = 0;
                ui_state = UI_SHOW_THRESHOLDS;
            }
            else if (S2_pressed)
            {
                ui_timer = 0;
                ui_state = UI_SHOW_TEMP;
            }
            break;
        
        case UI_SHOW_TEMP:
            if (ui_timer >= 1000)
            {
                ui_timer = 0;
                ui_state = UI_SHOW_LUM;
            }
            break;

        case UI_SHOW_LUM:
            if (ui_timer >= 1000)
            {
                ui_state = UI_NORMAL;
            }
            break;

        case UI_SHOW_THRESHOLDS:
            if (ui_timer >= 2000)
            {
                ui_state = UI_NORMAL;
            }
            else if (S1_pressed)
            {
                ui_state = UI_CONFIG_H;
            }
            break;

        case UI_CONFIG_H:
            if (S2_pressed)
            {
                clock_h++;

                if (clock_h > 23)
                    clock_h = 0;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_M;
            }

            break;


        case UI_CONFIG_M:
            if (S2_pressed)
            {
                clock_m++;

                if (clock_m > 59)
                    clock_m = 0;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_S;
            }
            break;


        case UI_CONFIG_S:
            if (S2_pressed)
            {
                clock_s++;

                if (clock_s > 59)
                    clock_s = 0;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_C;
            }
            break;

        case UI_CONFIG_C:
            if (S2_pressed)
            {
                ui_state = UI_CONFIG_C_EDIT;
            }
            else if (S1_pressed)
            {
                ui_state = UI_CONFIG_T;
            }
            break;

        case UI_CONFIG_C_EDIT:
            if (S2_pressed)
            {
                /*
                * Passar para alteração do alarme do relógio.
                */
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_T;
            }
            break;

        case UI_CONFIG_T:
            if (S2_pressed)
            {
                ui_state = UI_CONFIG_T_EDIT;
            }
            else if (S1_pressed)
            {
                ui_state = UI_CONFIG_L;
            }

            break;

        case UI_CONFIG_T_EDIT:
            if (S2_pressed)
            {
                alarm_temperature++;

                if (alarm_temperature > 50)
                    alarm_temperature = 0;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_L;
            }
            break;

        case UI_CONFIG_L:
            if (S2_pressed)
            {
                ui_state = UI_CONFIG_L_EDIT;
            }
            else if (S1_pressed)
            {
                ui_state = UI_CONFIG_ALARM;
            }
            break;

        case UI_CONFIG_L_EDIT:
            if (S2_pressed)
            {
                alarm_luminosity++;

                if (alarm_luminosity > 7)
                    alarm_luminosity = 0;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_ALARM;
            }
            break;

        case UI_CONFIG_ALARM:
            if (S2_pressed)
            {
                alarm_enabled = !alarm_enabled;
            }
            if (S1_pressed)
            {
                ui_state = UI_CONFIG_RESET;
            }
            break;

        case UI_CONFIG_RESET:
            if (S2_pressed)
            {
                Reset_Max_Min_Records();
            }
            if (S1_pressed)
            {
                ui_state = UI_NORMAL;
            }
            break;

        default:

            ui_state = UI_NORMAL;
            break;
    }
}