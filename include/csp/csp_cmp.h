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

#ifndef _CSP_CMP_H_
#define _CSP_CMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <csp/csp.h>

#define CSP_CMP_REQUEST 0x00
#define CSP_CMP_REPLY   0xff

#define CSP_CMP_IDENT 1
#define CSP_CMP_IDENT_REV_LEN  9
#define CSP_CMP_IDENT_DATE_LEN 12
#define CSP_CMP_IDENT_TIME_LEN 9

struct csp_cmp_message {
	uint8_t type;
	uint8_t code;
	union {
		struct {
			char hostname[CSP_HOSTNAME_LEN];
			char model[CSP_MODEL_LEN];
			char revision[CSP_CMP_IDENT_REV_LEN];
			char date[CSP_CMP_IDENT_DATE_LEN];
			char time[CSP_CMP_IDENT_TIME_LEN];
		} ident;
	};
} __attribute__ ((packed));

#define CMP_SIZE(_memb) (sizeof(((struct csp_cmp_message *)0)->_memb) + sizeof(uint8_t) + sizeof(uint8_t))

int csp_cmp(uint8_t node, uint32_t timeout, uint8_t code, int membsize, struct csp_cmp_message * msg);

#define CMP_MESSAGE(_code, _memb) \
static inline int csp_cmp_##_memb(uint8_t node, uint32_t timeout, struct csp_cmp_message * msg) { \
	return csp_cmp(node, timeout, _code, CMP_SIZE(_memb), msg); \
}

CMP_MESSAGE(CSP_CMP_IDENT, ident);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // _CSP_CMP_H_
