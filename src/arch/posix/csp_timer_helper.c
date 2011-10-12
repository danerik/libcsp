/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2011 Gomspace ApS (http://www.gomspace.com)
Copyright (C) 2011 AAUSAT3 Project (http://aausat3.space.aau.dk) 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#include "csp_timer_helper.h"

#ifdef _CSP_WINDOWS_
	#include <Windows.h>
#endif

static void append_ms(struct timespec* ts, int ms);

#ifdef _CSP_WINDOWS_
	int set_timeout(struct timespec* ts, int timeout) {
		uint32_t absoluteTimeout = (uint32_t)GetTickCount() + timeout;
		struct timespec tsTmp;

		memset(&tsTmp, 0, sizeof(struct timespec));
		append_ms(&tsTmp, absoluteTimeout);

		*ts = tsTmp;
		return 0;
	}
#else
	int set_timeout(struct timespec* ts, int timeout) {
		struct timespec tsTmp;
		int ret;

		if(ret = clock_gettime(CLOCK_REALTIME, &tsTmp) )
			return ret;

		append_ms(&tsTmp, timeout);
		*ts = tsTmp;

		return 0;
	}
#endif

static void append_ms(struct timespec* ts, int ms) {
	uint32_t sec = ms / 1000;
    uint32_t nsec = (ms - 1000 * sec) * 1000000;

    ts->tv_sec += sec;
    
    if (ts->tv_nsec + nsec > 1000000000)
        ts->tv_sec++;

    ts->tv_nsec = (ts->tv_nsec + nsec) % 1000000000;
}