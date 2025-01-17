/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2011 GomSpace ApS (http://www.gomspace.com)
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

#ifndef _CSP_PORT_H_
#define _CSP_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <csp/csp.h>

/** @brief Port states */
typedef enum {
	PORT_CLOSED = 0,
	PORT_OPEN = 1,
} csp_port_state_t;

/** @brief Port struct */
typedef struct {
	csp_port_state_t state;		 // Port state
	csp_socket_t * socket;		  // New connections are added to this socket's conn queue
} csp_port_t;

/**
 * Init ports array
 */
int csp_port_init(void);

csp_socket_t * csp_port_get_socket(unsigned int dport);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // _CSP_PORT_H_
