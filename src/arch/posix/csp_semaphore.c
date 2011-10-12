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


/* CSP includes */
#include <csp/csp.h>

#include "../csp_semaphore.h"
#include "csp_timer_helper.h"

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

int csp_mutex_lock(csp_mutex_t * mutex, int timeout) {
	if (mutex == NULL)
		return 0;

	csp_debug(CSP_LOCK, "Wait: %p timeout %u\r\n", mutex, timeout);

	struct timespec ts;
    if(csp_set_timeout(&ts, timeout))
		return CSP_SEMAPHORE_ERROR;

	if (pthread_mutex_timedlock(mutex, &ts) == 0) {
		return CSP_SEMAPHORE_OK;
	} else {
		return CSP_SEMAPHORE_ERROR;
	}
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

int csp_bin_sem_wait(csp_bin_sem_handle_t * sem, int timeout) {

	if (sem == NULL)
		return 0;

	csp_debug(CSP_LOCK, "Wait: %p timeout %u\r\n", sem, timeout);

    struct timespec ts;
    if(csp_set_timeout(&ts, timeout))
		return CSP_SEMAPHORE_ERROR;

    if (sem_timedwait(sem, (const struct timespec*)&ts) == 0) {
        return CSP_SEMAPHORE_OK;
    } else {
        return CSP_SEMAPHORE_ERROR;
    }
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