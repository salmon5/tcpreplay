/* $Id: $ */

/*
 * Copyright (c) 2001-2005 Aaron Turner.
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


#include "config.h"
#include "defines.h"
#include "common.h"
#include "../../lib/sll.h"
#include "../dlt.h"

#ifdef DEBUG
extern int debug;
#endif




/*
 * returns the L2 protocol (IP, ARP, etc)
 * or 0 for error
 */
u_int16_t
get_l2protocol(const u_char *pktdata, const int datalen, const int datalink)
{
    eth_hdr_t *eth_hdr;
    vlan_hdr_t *vlan_hdr;
    hdlc_hdr_t *hdlc_hdr;
    sll_hdr_t *sll_hdr;
    u_int16_t ether_type;

    assert(pktdata);
    assert(datalen);

    switch (datalink) {
    case DLT_RAW:
        return ETHERTYPE_IP;
        break;

    case DLT_EN10MB:
        eth_hdr = (eth_hdr_t *)pktdata;
        ether_type = ntohs(eth_hdr->ether_type);
        switch (ether_type) {
        case ETHERTYPE_VLAN: /* 802.1q */
            vlan_hdr = (vlan_hdr_t *)pktdata;
            return vlan_hdr->vlan_len;
        default:
            return ether_type; /* yes, return it in nbo */
        }
        break;

    case DLT_C_HDLC:
        hdlc_hdr = (hdlc_hdr_t *)pktdata;
        return hdlc_hdr->protocol;
        break;

    case DLT_LINUX_SLL:
        sll_hdr = (sll_hdr_t *)pktdata;
        return sll_hdr->sll_protocol;
        break;

    default:
        errx(1, "Unable to process unsupported DLT type: %s (0x%x)", 
             pcap_datalink_val_to_description(datalink), datalink);

    }

    return 0;

}

/*
 * returns the length in number of bytes of the L2 header, or -1 on error
 */
int
get_l2len(const u_char *pktdata, const int datalen, const int datalink)
{
    eth_hdr_t *eth_hdr;
    u_int16_t ether_type;
    
    assert(pktdata);
    assert(datalen);

    switch (datalink) {
    case DLT_RAW:
        /* pktdata IS the ip header! */
        return 0;
        break;

    case DLT_EN10MB:
        eth_hdr = (eth_hdr_t *)pktdata;
        ether_type = ntohs(eth_hdr->ether_type);
        switch (ether_type) {
        case ETHERTYPE_VLAN:            /* 802.1q */
            return LIBNET_802_1Q_H;
            break;
        default:              /* ethernet */
            return LIBNET_ETH_H;
            break;
        }
        break;
        
    case DLT_C_HDLC:
        return CISCO_HDLC_LEN;
        break;

    case DLT_LINUX_SLL:
        return SLL_HDR_LEN;
        break;

    default:
        errx(1, "Unable to process unsupported DLT type: %s (0x%x)", 
             pcap_datalink_val_to_description(datalink), datalink);
        break;
    }

    return -1; /* we shouldn't get here */
}

/*
 * returns a ptr to the ip header + data or NULL if it's not IP
 * we may use an extra buffer for the ip header (and above)
 * on stricly aligned systems where the layer 2 header doesn't
 * fall on a 4 byte boundry (like a standard ethernet header)
 *
 * Note: you can cast the result as an ip_hdr_t, but you'll be able 
 * to access data above the header minus any stripped L2 data
 */
const u_char *
get_ipv4(const u_char *pktdata, int datalen, int datalink, u_char **newbuff)
{
    const u_char *ip_hdr = NULL;
    int l2_len = 0;
    u_int16_t proto;

    assert(pktdata);
    assert(datalen);
    assert(*newbuff);

    l2_len = get_l2len(pktdata, datalen, datalink);

    /* sanity... datalen must be > l2_len + IP header len*/
    if (l2_len + LIBNET_IPV4_H > datalen) {
        dbg(1, "get_ipv4(): Layer 2 len > total packet len, hence no IP header");
        return NULL;
    }

    proto = get_l2protocol(pktdata, datalen, datalink);

    if (proto != ETHERTYPE_IP)
        return NULL;

#ifdef FORCE_ALIGN
    /* 
     * copy layer 3 and up to our temp packet buffer
     * for now on, we have to edit the packetbuff because
     * just before we send the packet, we copy the packetbuff 
     * back onto the pkt.data + l2len buffer
     * we do all this work to prevent byte alignment issues
     */
    if (l2_len % 4) {
        ip_hdr = *newbuff;
        memcpy(ip_hdr, (pktdata + l2_len), (datalen - l2_len));
    } else {

        /* we don't have to do a memcpy if l2_len lands on a boundry */
        ip_hdr = (pktdata + l2_len);
    }
#else
    /*
     * on non-strict byte align systems, don't need to memcpy(), 
     * just point to l2len bytes into the existing buffer
     */
    ip_hdr = (pktdata + l2_len);
#endif

    return ip_hdr;
}

/*
 * returns a pointer to the layer 4 header which is just beyond the IP header
 */
void *
get_layer4(const ip_hdr_t * ip_hdr)
{
    void *ptr;
    
    assert(ip_hdr);

    ptr = (u_int32_t *) ip_hdr + ip_hdr->ip_hl;
    return ((void *)ptr);
}


/*
 Local Variables:
 mode:c
 indent-tabs-mode:nil
 c-basic-offset:4
 End:
*/
