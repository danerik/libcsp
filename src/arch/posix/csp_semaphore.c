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

#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

/* CSP includes */
#include <csp/csp.h>

#include "../csp_semaphore.h"

int csp_mutex_create(csp_mutex_t * mutex) {
	csp_debug(CSP_LOCK, "Mutex init: %p\r\n", mutex);
	if (pthread_mutex_init(mutex, NULL) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
}

int csp_mutex_remove(csp_mutex_t * mutex) {
	if (pthread_mutex_destroy(mutex) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
}

int csp_mutex_lock(csp_mutex_t * mutex, uint32_t timeout) {

	int ret;
	struct timespec ts;
	uint32_t sec, nsec;

	csp_debug(CSP_LOCK, "Wait: %p timeout %"PRIu32"\r\n", mutex, timeout);

	if (timeout == CSP_INFINITY) {
		ret = pthread_mutex_lock(mutex);
	} else {
		if (clock_gettime(CLOCK_REALTIME, &ts))
			return CSP_SEMAPHORE_ERROR;

		sec = timeout / 1000;
		nsec = (timeout - 1000 * sec) * 1000000;

		ts.tv_sec += sec;

		if (ts.tv_nsec + nsec >= 1000000000)
			ts.tv_sec++;

		ts.tv_nsec = (ts.tv_nsec + nsec) % 1000000000;

		ret = pthread_mutex_timedlock(mutex, &ts);
	}

	if (ret != 0)
		return CSP_SEMAPHORE_ERROR;

	return CSP_SEMAPHORE_OK;
}

int csp_mutex_unlock(csp_mutex_t * mutex) {
	if (pthread_mutex_unlock(mutex) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
}

int csp_bin_sem_create(csp_bin_sem_handle_t * sem) {
	csp_debug(CSP_LOCK, "Semaphore init: %p\r\n", sem);
	if (sem_init(sem, 0, 1) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
}

int csp_bin_sem_remove(csp_bin_sem_handle_t * sem) {
	if (sem_destroy(sem) == 0)
		return CSP_SEMAPHORE_OK;
	else
		return CSP_SEMAPHORE_ERROR;
}

int csp_bin_sem_wait(csp_bin_sem_handle_t * sem, uint32_t timeout) {

	int ret;
	struct timespec ts;
	uint32_t sec, nsec;

	csp_debug(CSP_LOCK, "Wait: %p timeout %"PRIu32"\r\n", sem, timeout);

	if (timeout == CSP_INFINITY) {
		ret = sem_wait(sem);
	} else {
		if (clock_gettime(CLOCK_REALTIME, &ts))
			return CSP_SEMAPHORE_ERROR;
		
		sec = timeout / 1000;
		nsec = (timeout - 1000 * sec) * 1000000;

		ts.tv_sec += sec;

		if (ts.tv_nsec + nsec >= 1000000000)
			ts.tv_sec++;

		ts.tv_nsec = (ts.tv_nsec + nsec) % 1000000000;

		ret = sem_timedwait(sem, &ts);
	}

	if (ret != 0)
		return CSP_SEMAPHORE_ERROR;

	return CSP_SEMAPHORE_OK;
}

int csp_bin_sem_post(csp_bin_sem_handle_t * sem) {
	CSP_BASE_TYPE dummy = 0;
	return csp_bin_sem_post_isr(sem, &dummy);
}

int csp_bin_sem_post_isr(csp_bin_sem_handle_t * sem, CSP_BASE_TYPE * task_woken) {
	csp_debug(CSP_LOCK, "Post: %p\r\n", sem);
	*task_woken = 0;

	int value;
	sem_getvalue(sem, &value);
	if (value > 0)
		return CSP_SEMAPHORE_OK;

	if (sem_post(sem) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
}
