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

#ifndef _CSP_TRANSPORT_H_
#define _CSP_TRANSPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/** ARRIVING SEGMENT */
void csp_udp_new_packet(csp_conn_t * conn, csp_packet_t * packet);
void csp_rdp_new_packet(csp_conn_t * conn, csp_packet_t * packet);

/** RDP: USER REQUESTS */
int csp_rdp_connect(csp_conn_t * conn, uint32_t timeout);
int csp_rdp_allocate(csp_conn_t * conn);
int csp_rdp_close(csp_conn_t * conn);
void csp_rdp_conn_print(csp_conn_t * conn);
int csp_rdp_send(csp_conn_t * conn, csp_packet_t * packet, uint32_t timeout);
int csp_rdp_check_ack(csp_conn_t * conn);
void csp_rdp_check_timeouts(csp_conn_t * conn);
void csp_rdp_flush_all(csp_conn_t * conn);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _CSP_TRANSPORT_H_ */
