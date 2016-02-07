/*****************************************************************************
 * This file is part of libmicrodns.
 *
 * Copyright © 2014-2015 VideoLabs SAS
 *
 * Author: Jonathan Calmels <jbjcalmels@gmail.com>
 *
 *****************************************************************************
 * libmicrodns is released under LGPLv2.1 (or later) and is also available
 * under a commercial license.
 *****************************************************************************
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef MICRODNS_MDNS_H
#define MICRODNS_MDNS_H

/**
 * @file microdns.h
 * @brief main functions to listen and announce services
 */

#include <stdbool.h>

#include "rr.h"

# ifdef __cplusplus
extern "C" {
# endif

struct mdns_ctx;

#define MDNS_PORT        5353
#define MDNS_ADDR_IPV4   "224.0.0.251"
#define MDNS_ADDR_IPV6   "FF02::FB"

#define GET_RCODE(x)    (x&0x000f)
#define SET_RCODE(x,c)  (x|(c&0x000f))

#define GET_OPCODE(x)   (x&0x7800)
#define SET_OPCODE(x,c) (x|(c&0x7800))

enum mdns_hdr_flag {
       FLAG_QR = 1 << 15, // Question/Response
       FLAG_AA = 1 << 10, // Authoritative Answer
       FLAG_TC = 1 <<  9, // Truncated Response
       FLAG_RD = 1 <<  8, // Recursion Desired
       FLAG_RA = 1 <<  7, // Recursion Available
       FLAG_Z  = 1 <<  6, // Reserved
       FLAG_AD = 1 <<  5, // Authentic Data
       FLAG_CD = 1 <<  4, // Checking Disabled
};

struct mdns_hdr {
        uint16_t id;
        uint16_t flags;
        uint16_t num_qn;
        uint16_t num_ans_rr;
        uint16_t num_auth_rr;
        uint16_t num_add_rr;
};

typedef void (*mdns_callback)(void*, int, const struct rr_entry *);

/**
 * \return true if the listener should be stopped
 */
typedef bool (*mdns_stop_func)(void*);

/**
 * @brief Allocates and initialize a new mdns context
 *
 * @param ctx Returns the allocated context for the library
 * @param addr Address to listen to
 * @param port Port to listen on
 *
 * @return 0 if success
 */
extern int mdns_init(struct mdns_ctx **ctx, const char *addr, unsigned short port);
extern int mdns_destroy(struct mdns_ctx *ctx);
extern int mdns_entries_send(const struct mdns_ctx *ctx, const struct mdns_hdr *hdr, const struct rr_entry *entries);
extern void mdns_entries_print(const struct rr_entry *);

extern int mdns_strerror(int, char *, size_t);

extern int mdns_listen(const struct mdns_ctx *ctx, const char *const names[],
                       unsigned int nb_names, enum rr_type type,
                       unsigned int interval, mdns_stop_func stop,
                       mdns_callback callback, void *p_cookie);
extern int mdns_announce(struct mdns_ctx *ctx, const char *service, enum rr_type,
        mdns_callback callback, void *p_cookie);
extern int mdns_serve(struct mdns_ctx *ctx, mdns_stop_func stop, void *p_cookie);

# ifdef __cplusplus
}
# endif

#endif /* MICRODNS_MDNS_H */