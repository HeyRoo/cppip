/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 15:15:54
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 19:44:49
 */
#ifndef __ETHER_H
#define __ETHER_H

#include <string.h>

#define ETH_HRD_SZ	sizeof(struct ether)
#define ETH_ALEN	6	/* ether address len */

#define ETH_P_IP	0x0800
#define ETH_P_ARP	0x0806
#define ETH_P_RARP	0x8035

struct ether {
	unsigned char eth_dst[ETH_ALEN];	/* destination ether addr */
	unsigned char eth_src[ETH_ALEN];	/* source ether addr */
	unsigned short eth_pro;			/* packet type ID */
	unsigned char* eth_data;		/* data field */
};


#endif	/* ether.h */
