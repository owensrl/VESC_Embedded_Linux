/*
	Copyright 2017 Ryan Owens

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * timers.c
 *
 * Important: Must compile with -lrt otherwise will have linking errors
 *
 *  Created on: 3 apr 2017
 *      Authors: Anirudh, Ryan
 */
 
#ifndef TIMERS_C
#define TIMERS_C

#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>
#include <signal.h>

 // Returns NULL on error
timer_t* new_timer() {
    timer_t* timerid = (timer_t*)malloc(sizeof(timer_t));
    struct sigevent sev;
    sev.sigev_notify = SIGEV_NONE;
    int ret = timer_create(CLOCK_MONOTONIC, &sev, timerid);
    if(ret == -1) {
        free(timerid);
        return NULL;
    } else {
        return timerid;
    }
}

// ms is milliseconds, less than 1000
// longer than 1000ms not supported
void start_timer(timer_t* timerid, int ms) {
    int sec = ms / 1000;
    int ns = (ms % 1000) * 1000 * 1000; // convert to nanoseconds
    struct itimerspec spec = {{0, 0}, {sec, ns}};
    timer_settime(*timerid, 0, &spec, NULL);
}

// false: still running
// true:  timer expired
// If timer has expired, it must be started again with
// start_timer before using
bool check_timer(timer_t* timerid) {
    struct itimerspec spec;
    timer_gettime(*timerid, &spec);
    return spec.it_value.tv_sec == 0 && spec.it_value.tv_nsec == 0;
}

void delete_timer(timer_t* timerid) {
    timer_delete(*timerid);
    free(timerid);
}
#endif
