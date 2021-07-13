/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:36:45
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 14:51:25
 */
#ifndef __IP_H__
#define __IP_H__

#include "ether.h"

/* IP Packet Format */
#define IP_ALEN 4

#define IP_VERSION_4	4

#define IP_FRAG_RS	0x8000
#define IP_FRAG_DF	0x4000
#define IP_FRAG_MF	0x2000
#define IP_FRAG_OFF	0x1fff
#define IP_FRAG_MASK	(IP_FRAG_OFF | IP_FRAG_MF)

#define IP_P_IP		0
#define IP_P_ICMP	1
#define IP_P_IGMP	2
#define IP_P_TCP	6
#define IP_P_EGP	8
#define IP_P_UDP	17
#define IP_P_OSPF	89
#define IP_P_RAW	255
#define IP_P_MAX	256

#define MULTICAST(netip) ((0x000000f0 & (netip)) == 0x000000e0)

struct ip {
	unsigned char	ip_hlen:4,	/* header length(in 4-octet's) */
			ip_ver:4;	/* version */
	unsigned char ip_tos;		/* type of service */
	unsigned short ip_len;		/* total ip packet data length */
	unsigned short ip_id;		/* datagram id */
	unsigned short ip_fragoff;	/* fragment offset(in 8-octet's) */
	unsigned char ip_ttl;		/* time to live, in gateway hops */
	unsigned char ip_pro;		/* L4 protocol */
	unsigned short ip_cksum;	/* header checksum */
	unsigned int ip_src;		/* source address */
	unsigned int ip_dst;		/* dest address */
	unsigned char* ip_data;	/* data field */
};

#define IP_HRD_SZ sizeof(struct ip)

#define ipver(ip) ((ip)->ip_ver)
#define iphlen(ip) ((ip)->ip_hlen << 2)
#define ipdlen(ip) ((ip)->ip_len - iphlen(iphdr))
#define ipndlen(nip) (_ntohs((nip)->ip_len) - iphlen(nip))
#define ipdata(ip) ((unsigned char *)(ip) + iphlen(ip))
#define ipoff(ip) ((((ip)->ip_fragoff) & IP_FRAG_OFF) * 8)
#define pkb2ip(pkb) ((struct ip *)((pkb)->pk_data + ETH_HRD_SZ))

#endif