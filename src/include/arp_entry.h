/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 17:42:38
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 18:35:43
 */
#ifndef __ARP_ENTRY_H__
#define __ARP_ENTRY_H__

#include <list>
#include "ether.h"
#include "pkb.h"

class ArpEntry
{
public:
    // static int order;
    /* data */
    struct netdev *ae_dev;			/* associated net interface */
	int ae_retry;				/* arp reuqest retrying times */
	int ae_ttl;				/* entry timeout */
	unsigned int ae_state;			/* entry state */
	unsigned short ae_pro;			/* L3 protocol supported by arp */
	unsigned int ae_ipaddr;			/* L3 protocol address(ip) */
	unsigned char ae_hwaddr[ETH_ALEN];	/* L2 protocol address(ethernet) */

    std::list<PKB*> que;
public:
    ArpEntry(/* args */);
    ~ArpEntry();
    void send_queue();
    void drop_queue();
};



#endif