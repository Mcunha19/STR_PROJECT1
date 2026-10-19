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
