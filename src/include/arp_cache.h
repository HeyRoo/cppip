/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:54:17
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 18:47:36
 */
#ifndef __ARP_CACHE_H__
#define __ARP_CACHE_H__

#include <mutex>
#include <list>
#include "ether.h"
#include "arp.h"
#include "arp_entry.h"


/* arp cache */
#define ARP_CACHE_SZ	20
#define ARP_TIMEOUT	600	/* 10 minutes */
#define ARP_WAITTIME	1

/* arp entry state */
#define ARP_FREE	1
#define ARP_WAITING	2
#define ARP_RESOLVED	3

class ArpCache
{
private:
    std::list<ArpEntry*> cache;
	std::mutex cache_lock;
public:
	static ArpCache* instance();
	ArpEntry* lookup(unsigned short pro, unsigned int ipaddr);
	int insert(struct netdev *nd, unsigned short pro, unsigned int ipaddr, unsigned char *hwaddr);
	ArpEntry* alloc_entry();

private:
	ArpCache();
    ~ArpCache();
};




#endif