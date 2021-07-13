/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 22:48:02
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 11:49:29
 */
#ifndef __ROUTE_CACHE_H__
#define __ROUTE_CACHE_H__
#include <mutex>
#include <list>
#include "route_entry.h"

class RouteCache
{
private:
    std::list<RouteEntry*> cache;
	std::mutex cache_lock;
public:
	static RouteCache* instance();
	RouteEntry* lookup(unsigned int ipaddr);
	int insert(unsigned int net, unsigned int netmask, unsigned int gw, int metric, unsigned int flags, struct netdev *dev);
	RouteEntry* alloc_entry(unsigned int net, unsigned int netmask, unsigned int gw, int metric, unsigned int flags, struct netdev *dev);

private:
    RouteCache(/* args */);
    ~RouteCache();
};







#endif