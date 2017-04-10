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
 * timers.h
 * Generic abstraction for POSIX timers
 * Only supports one-shot timers
 * 
 * Important: Must compile with -lrt otherwise will have linking errors
 *
 *  Created on: 3 apr 2017
 *      Authors: Anirudh, Ryan
 */
 
#ifndef TIMERS_H
#define TIMERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif
	
timer_t* new_timer();
void start_timer(timer_t* timerid, int ms);
bool check_timer(timer_t* timerid);
void delete_timer(timer_t* timerid);

#ifdef __cplusplus
}
#endif
#endif