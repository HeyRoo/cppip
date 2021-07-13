/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:16:39
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 18:27:34
 */
#ifndef __ARP_H__
#define __ARP_H__

#define ARP_ETHERNET
#define ARP_IP

struct arp {
	unsigned short arp_hrd;		/* hardware address type */
	unsigned short arp_pro;		/* protocol address type */
	unsigned char arp_hrdlen;	/* hardware address lenth */
	unsigned char arp_prolen;	/* protocol address lenth */
	unsigned short arp_op;		/* ARP opcode(command) */
#if defined(ARP_ETHERNET) && defined(ARP_IP)	/* only support ethernet & ip */
	unsigned char arp_sha[ETH_ALEN];	/* sender hw addr */
	unsigned int arp_sip;			/* send ip addr */
	unsigned char arp_tha[ETH_ALEN];	/* target hw addr */
	unsigned int arp_tip;			/* target ip addr */
#else
	unsigned char arp_data[0];		/* arp data field */
#endif
};

#define ARP_HRD_SZ sizeof(struct arp)

/* arp format */
#define ARP_HRD_ETHER		1

#define ARP_OP_REQUEST		1	/* ARP request */
#define ARP_OP_REPLY		2	/* ARP reply */
#define ARP_OP_RREQUEST		3	/* RARP request */
#define ARP_OP_RREPLY		4	/* RARP reply */
#define ARP_OP_INREQUEST	8	/* InARP request */
#define ARP_OP_INREPLY		9	/* InARP reply */

#define ARP_REQ_RETRY	4

#endif