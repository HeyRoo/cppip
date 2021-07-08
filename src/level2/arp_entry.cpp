/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 17:43:39
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 19:02:33
 */
#include "arp_entry.h"
#include "arp_cache.h"
#include "arp.h"
#include "pkb.h"
#include "dbg.h"

ArpEntry::ArpEntry(/* args */)
{
    ae_dev = nullptr;			/* associated net interface */
	ae_retry = ARP_REQ_RETRY;				/* arp reuqest retrying times */
	ae_ttl = ARP_WAITTIME;				/* entry timeout */
	ae_state = ARP_WAITING;			/* entry state */
	ae_pro = ETH_P_IP;			/* L3 protocol supported by arp */
	ae_ipaddr = 0;			/* L3 protocol address(ip) */
	// ae_hwaddr = ;	/* L2 protocol address(ethernet) */
}

ArpEntry::~ArpEntry()
{
}

void ArpEntry::send_queue(){
	PKB* pkb;
	while (!que.empty())
	{
		pkb = que.front();
		que.pop_front();
		dbg("send pending packet");
		// netdev_tx(ae->ae_dev, pkb, pkb->pk_len - ETH_HRD_SZ, pkb->pk_pro, ae->ae_hwaddr);
	}
	
}

void ArpEntry::drop_queue(){
	PKB* pkb;
	while (!que.empty())
	{
		pkb = que.front();
		que.pop_front();
		dbg("drop pending packet");
		delete pkb;
		// netdev_tx(ae->ae_dev, pkb, pkb->pk_len - ETH_HRD_SZ, pkb->pk_pro, ae->ae_hwaddr);
	}
	
}