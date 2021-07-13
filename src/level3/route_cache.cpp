/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 22:49:32
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 14:10:48
 */
#include "route_cache.h"
#include "net_if.h"
#include "lo_eth.h"
#include "vir_eth.h"

extern LoEth* loop;
extern VirEth* veth;

RouteCache::RouteCache(/* args */)
{
    insert(LOCALNET(loop->dev), loop->dev->net_mask, 0, 0, RT_LOCALHOST, loop->dev);
    insert(veth->dev->net_ipaddr, 0xffffffff, 0, 0, RT_LOCALHOST, loop->dev);
    insert(LOCALNET(veth->dev), veth->dev->net_mask, 0, 0, RT_NONE, veth->dev);

    insert(0, 0, veth->tap->tap_nic->dev.net_ipaddr, 0, RT_DEFAULT, veth->dev);

}

RouteCache::~RouteCache()
{
    cache_lock.lock();
    while(!cache.empty()){
        RouteEntry* re = cache.back();
        cache.pop_back();
        delete re;
    }
    cache_lock.unlock();
}

RouteCache* RouteCache::instance(){
    static RouteCache inst;
	return &inst;
}

int RouteCache::insert(unsigned int net, unsigned int netmask, unsigned int gw, int metric, unsigned int flags, struct netdev *dev){
	RouteEntry* re = nullptr;
	re = alloc_entry(net, netmask, gw, metric, flags, dev);
	
	if(re == nullptr){
		return -1;
	}
    cache_lock.lock();
    if(cache.empty()){
        cache.push_back(re);
    }else{
        for(auto it = cache.begin(); it != cache.end(); it++){
            if(re->rt_netmask >= (*it)->rt_netmask){
                cache.insert(it, re);
                break;
            }
        }
    }
    cache_lock.unlock();
	return 0;
}

/**
 * @name: alloc_entry
 * @msg: 分配一个route entry
 * @param {unsigned int} net
 * @param {unsigned int} netmask
 * @param {unsigned int} gw
 * @param {int} metric
 * @param {unsigned int} flags
 * @param {struct netdev} *dev
 * @return {*} 返回的entry 并不在cache中
 */
RouteEntry* RouteCache::alloc_entry(unsigned int net, unsigned int netmask, unsigned int gw, int metric, unsigned int flags, struct netdev *dev){
	RouteEntry *rt = new RouteEntry();
	rt->rt_net = net;
	rt->rt_netmask = netmask;
	rt->rt_gw = gw;
	rt->rt_metric = metric;
	rt->rt_flags = flags;
	rt->rt_dev = dev;
	return rt;
}

RouteEntry* RouteCache::lookup(unsigned int ipaddr){
    RouteEntry* re = nullptr;
    cache_lock.lock();
    for(auto it = cache.begin(); it != cache.end(); it++){
        if(((*it)->rt_netmask & ipaddr) == ((*it)->rt_netmask & (*it)->rt_net)){
            re = (*it);
            break;
        }
    }
    cache_lock.unlock();
    return re;
}