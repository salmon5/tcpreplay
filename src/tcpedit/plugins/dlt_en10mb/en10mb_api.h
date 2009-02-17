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



#ifndef _DLT_en10mb_API_H_
#define _DLT_en10mb_API_H_

#include "tcpedit_types.h"
#include "plugins_types.h"
#include "en10mb_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief EN10MB (Ethernet) Plugin API functions for modifying the 802.3 Ethernet header
 *
 * setters always return TCPEDIT_OK on success or TCPEDIT_ERROR 
 * if there is a problem.  You can use tcpedit_geterr() to get the reason
 * for the failure
 */
int tcpedit_en10mb_set_mac(tcpedit_t *tcpedit, char *mac, tcpedit_mac_mask mask);
int tcpedit_en10mb_set_vlan_mode(tcpedit_t *tcpedit, tcpedit_vlan vlan);
int tcpedit_en10mb_set_vlan_tag(tcpedit_t *tcpedit, u_int16_t tag);
int tcpedit_en10mb_set_vlan_priority(tcpedit_t *tcpedit, u_int8_t priority);
int tcpedit_en10mb_set_vlan_cfi(tcpedit_t *tcpedit, u_int8_t cfi);

#ifdef __cplusplus
}
#endif

#endif

