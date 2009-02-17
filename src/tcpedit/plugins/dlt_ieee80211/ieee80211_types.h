/* $Id$ */

/*
 * Copyright (c) 2009 Aaron Turner.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright owners nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _DLT_ieee80211_TYPES_H_
#define _DLT_ieee80211_TYPES_H_

#include "plugins_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/* 802.11 packet header w/ 3 addresses (non-WDS) */
typedef struct {
    u_int16_t frame_control;
/* version is first two bytes */
#define ieee80211_FC_VERSION_MASK   0x0300

/* type is second 2 bytes */
#define ieee80211_FC_TYPE_MASK      0x0F00
#define ieee80211_FC_TYPE_DATA      0x0800
#define ieee80211_FC_TYPE_MGMT      0x0000
#define ieee80211_FC_TYPE_CONTROL   0x0400

/* subtype is the 4 high bytes */
#define ieee80211_FC_SUBTYPE_MASK   0xF000
#define ieee80211_FC_SUBTYPE_QOS    0x8000 /* high bit is QoS, but there are sub-sub types for QoS */
#define ieee80211_FC_SUBTYPE_NULL   0xC000 /* no data */

/* Direction */
#define ieee80211_FC_TO_DS_MASK     0x0001
#define ieee80211_FC_FROM_DS_MASK   0x0002

/* Flags */
#define ieee80211_FC_MORE_FRAG      0x0004
#define ieee80211_FC_RETRY_MASK     0x0008
#define ieee80211_FC_PWR_MGMT_MASK  0x0010
#define ieee80211_FC_MORE_DATA_MASK 0x0020
#define ieee80211_FC_WEP_MASK       0x0040
#define ieee80211_FC_ORDER_MASK     0x0080
    u_int16_t duration;
    u_char addr1[6];
    u_char addr2[6];
    u_char addr3[6];
    u_int16_t fragid;
} ieee80211_hdr_t;

typedef struct {
    u_int16_t frame_control;
    u_int16_t duration;
    u_char addr1[6];
    u_char addr2[6];
    u_char addr3[6];
    u_char addr4[6];
    u_int16_t fragid;
} ieee80211_addr4_hdr_t;


#define ieee80211_USE_4(frame_control)                                          \
    (frame_control & (ieee80211_FC_TO_DS_MASK + ieee80211_FC_FROM_DS_MASK)) ==   \
    (ieee80211_FC_TO_DS_MASK + ieee80211_FC_FROM_DS_MASK)

/*
 * FIXME: structure to hold any data parsed from the packet by the decoder.
 * Example: Ethernet VLAN tag info
 */
typedef struct {
    /* dummy entry for SunPro compiler which doesn't like empty structs */    
    int dummy;
} ieee80211_extra_t;

/* 
 * FIXME: structure to hold any data in the tcpeditdlt_plugin_t->config 
 * Things like: 
 * - Parsed user options
 * - State between packets
 * - Note, you should only use this for the encoder function, decoder functions should place
 *   "extra" data parsed from the packet in the tcpeditdlt_t->decoded_extra buffer since that 
 *   is available to any encoder plugin.
 */
typedef struct {
    /* dummy entry for SunPro compiler which doesn't like empty structs */    
    int dummy;
} ieee80211_config_t;

#ifdef __cplusplus
}
#endif

#endif

