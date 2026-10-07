/*
 * File:   Time.c
 * Author: Tomás Seabra
 *
 */

#include "Time.h"

int Time_reset(time_t* time) {
    if (!time) {
        return -1;
    }
    time->hour = 0;
    time->minute = 0;
    time->second = 0;
    return 0;
}

int Time_increment(time_t *time) {
    if (!time) {
        return -1;
    }
    
    time->second++;

    if (time->second >= 60) {
        time->second = 0;
        time->minute++;

        if (time->minute >= 60) {
            time->minute = 0;
            time->hour++;

            if (time->hour >= 100) {
                time->hour = 0;
            }
        }
    }
    return 0;
}

int Time_setHours(time_t *time, uint8_t hours) {
    if (!time) {
        return -1;
    }
    
    if (hours <= 99) {
        time->hour = hours;
    }
    return 0;
}

int Time_setMinutes(time_t *time, uint8_t minutes) {
    if (!time) {
        return -1;
    }
    
    if (minutes <= 59) {
        time->minute = minutes;
    }
    return 0;
}

int Time_setSeconds(time_t *time, uint8_t seconds) {
    if (!time) {
        return -1;
    }
    
    if (seconds <= 59) {
        time->second = seconds;
    }
    return 0;
}
