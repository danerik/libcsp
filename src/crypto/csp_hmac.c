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

/* Hash-based Message Authentication Code - based on code from libtom.org */

#include <stdint.h>
#include <string.h>

/* CSP includes */
#include <csp/csp.h>

#include "csp_hmac.h"
#include "csp_sha1.h"

#ifdef CSP_USE_HMAC

#define HMAC_KEY_LENGTH	16

/* HMAC key */
static uint8_t csp_hmac_key[HMAC_KEY_LENGTH];

/* HMAC state structure */
typedef struct {
	csp_sha1_state	md;
	uint8_t		key[SHA1_BLOCKSIZE];
} hmac_state;

int csp_hmac_init(hmac_state * hmac, const uint8_t * key, uint32_t keylen) {
	uint32_t i;
	uint8_t buf[SHA1_BLOCKSIZE];

	/* NULL pointer and key check */
	if (!hmac || !key || keylen < 1)
		return -1;

	/* Make sure we have a large enough key */
	if(keylen > SHA1_BLOCKSIZE) {
		csp_sha1_memory(key, keylen, hmac->key);
		if(SHA1_DIGESTSIZE < SHA1_BLOCKSIZE)
			memset((hmac->key) + SHA1_DIGESTSIZE, 0, (size_t)(SHA1_BLOCKSIZE - SHA1_DIGESTSIZE));
	} else {
		memcpy(hmac->key, key, (size_t)keylen);
		if(keylen < SHA1_BLOCKSIZE)
			memset((hmac->key) + keylen, 0, (size_t)(SHA1_BLOCKSIZE - keylen));
	}

	/* Create the initial vector */
	for(i = 0; i < SHA1_BLOCKSIZE; i++)
	   buf[i] = hmac->key[i] ^ 0x36;

	/* Prepend to the hash data */
	csp_sha1_init(&hmac->md);
	csp_sha1_process(&hmac->md, buf, SHA1_BLOCKSIZE);

	return 0;
}

int csp_hmac_process(hmac_state * hmac, const uint8_t * in, uint32_t inlen) {

	/* NULL pointer check */
	if (!hmac || !in)
		return -1;

	/* Process data */
	csp_sha1_process(&hmac->md, in, inlen);

	return 0;
}

int csp_hmac_done(hmac_state * hmac, uint8_t * out) {

	uint32_t i;
	uint8_t buf[SHA1_BLOCKSIZE];
	uint8_t isha[SHA1_DIGESTSIZE];

	if (!hmac || !out)
		return -1;

	/* Get the hash of the first HMAC vector plus the data */
	csp_sha1_done(&hmac->md, isha);

	/* Create the second HMAC vector vector */
	for(i = 0; i < SHA1_BLOCKSIZE; i++)
		buf[i] = hmac->key[i] ^ 0x5C;

	/* Now calculate the outer hash */
	csp_sha1_init(&hmac->md);
	csp_sha1_process(&hmac->md, buf, SHA1_BLOCKSIZE);
	csp_sha1_process(&hmac->md, isha, SHA1_DIGESTSIZE);
	csp_sha1_done(&hmac->md, buf);

	/* Copy to output  */
	for (i = 0; i < SHA1_DIGESTSIZE; i++)
		out[i] = buf[i];

	return 0;
}

int csp_hmac_memory(const uint8_t * key, uint32_t keylen, const uint8_t * data, uint32_t datalen, uint8_t * hmac) {
	hmac_state state;

	/* NULL pointer check */
	if (!key || !data || !hmac)
		return -1;

	/* Init HMAC state */
	if (csp_hmac_init(&state, key, keylen) != 0)
		return -1;

	/* Process data */
	if (csp_hmac_process(&state, data, datalen) != 0)
		return -1;

	/* Output HMAC */
	if (csp_hmac_done(&state, hmac) != 0)
		return -1;

	return 0;
}

int csp_hmac_set_key(char * key, uint32_t keylen) {

	/* Use SHA1 as KDF */
	uint8_t hash[SHA1_DIGESTSIZE];
	csp_sha1_memory((uint8_t *)key, keylen, hash);

	/* Copy key */
	memcpy(csp_hmac_key, hash, HMAC_KEY_LENGTH);

	return 0;

}

int csp_hmac_append(csp_packet_t * packet) {

	/* NULL pointer check */
	if (packet == NULL)
		return -1;

	uint8_t hmac[SHA1_DIGESTSIZE];

	/* Calculate HMAC */
	csp_hmac_memory(csp_hmac_key, HMAC_KEY_LENGTH, packet->data, packet->length, hmac);

	/* Truncate hash and copy to packet */
	memcpy(&packet->data[packet->length], hmac, CSP_HMAC_LENGTH);
	packet->length += CSP_HMAC_LENGTH;

	return 0;

}

int csp_hmac_verify(csp_packet_t * packet) {

	/* NULL pointer check */
	if (packet == NULL)
		return -1;

	uint8_t hmac[SHA1_DIGESTSIZE];

	/* Calculate HMAC */
	csp_hmac_memory(csp_hmac_key, HMAC_KEY_LENGTH, packet->data, packet->length - CSP_HMAC_LENGTH, hmac);

	/* Compare calculated HMAC with packet header */
	if (memcmp(&packet->data[packet->length] - CSP_HMAC_LENGTH, hmac, CSP_HMAC_LENGTH) != 0) {
		/* HMAC failed */
		return -1;
	} else {
		/* Strip HMAC */
		packet->length -= CSP_HMAC_LENGTH;
		return 0;
	}

}

#endif // CSP_USE_HMAC
