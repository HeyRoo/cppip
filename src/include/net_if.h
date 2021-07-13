/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-28 16:29:33
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 13:04:52
 */
#ifndef __NET_IF_H__
#define __NET_IF_H__

#include <list>
#include "route_entry.h"

#define NETDEV_ALEN	6
#define NETDEV_NLEN	16	/* IFNAMSIZ */

struct netstats {
	unsigned int rx_packets;
	unsigned int tx_packets;
	unsigned int rx_errors;
	unsigned int tx_errors;
	unsigned int rx_bytes;
	unsigned int tx_bytes;
};

struct netdev {
	/* tap device information */
	int net_mtu;
	unsigned int net_ipaddr;		/* dev binding ip address */
	unsigned int net_mask;			/* netmask */
	unsigned char net_hwaddr[NETDEV_ALEN];	/* hardware address */
	unsigned char net_name[NETDEV_NLEN];	/* device name */
	// struct netdev_ops *net_ops;		/* Nic Operation */
	struct netstats net_stats;		/* protocol independent statistic */
	// std::list<struct netdev>* net_list;		/* net device list */
};


/* packet info */
struct pkhdr {
	// struct list_head pk_list;	/* for ip fragment or arp waiting list */
	unsigned short pk_pro;		/* ethernet packet type ID */
	unsigned short pk_type;		/* packet hardware address type */
	int pk_len;
	int pk_refcnt;
	struct netdev *pk_indev;
	RouteEntry *pk_rtdst;
	struct sock *pk_sk;
	// unsigned char *pk_data;
};

/* packet hardware address type */
#define PKT_NONE	0
#define PKT_LOCALHOST	1
#define PKT_OTHERHOST	2
#define PKT_MULTICAST	3
#define PKT_BROADCAST	4

#define LOCALNET(dev) ((dev)->net_ipaddr & (dev)->net_mask)

#endif