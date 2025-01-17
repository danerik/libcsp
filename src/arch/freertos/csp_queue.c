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

#include <stdint.h>

/* FreeRTOS includes */
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/* CSP includes */
#include <csp/csp.h>

#include "../csp_queue.h"

csp_queue_handle_t csp_queue_create(int length, size_t item_size) {
	return xQueueCreate(length, item_size);
}

void csp_queue_remove(csp_queue_handle_t queue) {
	vQueueDelete(queue);
}

int csp_queue_enqueue(csp_queue_handle_t handle, void * value, uint32_t timeout) {
	if (timeout != CSP_MAX_DELAY)
		timeout = timeout / portTICK_RATE_MS;
	return xQueueSendToBack(handle, value, timeout);
}

int csp_queue_enqueue_isr(csp_queue_handle_t handle, void * value, CSP_BASE_TYPE * task_woken) {
	return xQueueSendToBackFromISR(handle, value, (signed CSP_BASE_TYPE *)task_woken);
}

int csp_queue_dequeue(csp_queue_handle_t handle, void * buf, uint32_t timeout) {
	if (timeout != CSP_MAX_DELAY)
		timeout = timeout / portTICK_RATE_MS;
	return xQueueReceive(handle, buf, timeout);
}

int csp_queue_dequeue_isr(csp_queue_handle_t handle, void * buf, CSP_BASE_TYPE * task_woken) {
	return xQueueReceiveFromISR(handle, buf, (signed CSP_BASE_TYPE *)task_woken);
}

int csp_queue_size(csp_queue_handle_t handle) {
	return uxQueueMessagesWaiting(handle);
}

int csp_queue_size_isr(csp_queue_handle_t handle) {
	return uxQueueMessagesWaitingFromISR(handle);
}
