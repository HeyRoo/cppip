/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:59:34
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 19:45:51
 */
#include "arp_cache.h"
#include "dbg.h"


ArpCache::ArpCache(/* args */)
{
}

ArpCache::~ArpCache()
{
}

ArpCache* ArpCache::instance(){
	static ArpCache inst;
	return &inst;
}

ArpEntry* ArpCache::lookup(unsigned short pro, unsigned int ipaddr){
	ArpEntry* ret = nullptr;
	cache_lock.lock();
	dbg("lookup arp", pro, ipaddr);
	for(auto i = cache.begin(); i != cache.end(); i++){
		if((*i)->ae_state == ARP_FREE){
			continue;
		}
		if((*i)->ae_pro == pro && (*i)->ae_ipaddr == ipaddr){
			ret = (*i);
			break;
		}
	}
	cache_lock.unlock();
	return ret;
}
/**
 * @name: 分配一个entry的空间
 * @msg: 
 * @param {*}
 * @return {*} 返回的entry已经存在于cache中
 */
ArpEntry* ArpCache::alloc_entry(){
	ArpEntry* ae = nullptr;
	cache_lock.lock();
	/* find first free item */
	if(cache.size() >= ARP_CACHE_SZ){
		for(auto it = cache.begin(); it != cache.end(); it++){
			if((*it)->ae_state == ARP_FREE){
				ae = (*it);
			}
		}
		if(ae == nullptr){
			dbg("arp cache is full");
			cache_lock.unlock();
			return nullptr;
		}
		ae->ae_dev = nullptr;
		ae->ae_retry = ARP_REQ_RETRY;
		ae->ae_ttl = ARP_WAITTIME;
		ae->ae_state = ARP_WAITING;
		ae->ae_pro = ETH_P_IP;		/* default protocol */
	}else{
		ae = new ArpEntry();
	}
	cache_lock.unlock();
	return ae;
}

int ArpCache::insert(struct netdev *nd, unsigned short pro, unsigned int ipaddr, unsigned char *hwaddr)
{
	ArpEntry* ae = nullptr;
	ae = alloc_entry();
	
	if(ae == nullptr){
		return -1;
	}
	ae->ae_dev = nd;
	ae->ae_pro = pro;
	ae->ae_ttl = ARP_TIMEOUT;
	ae->ae_ipaddr = ipaddr;
	ae->ae_state = ARP_RESOLVED;
	memcpy(ae->ae_hwaddr, hwaddr, ETH_ALEN);
	return 0;
}