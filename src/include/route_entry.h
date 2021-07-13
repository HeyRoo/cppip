/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 22:50:35
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-12 23:29:37
 */
#ifndef __ROUTE_ENTRY_H__
#define __ROUTE_ENTRY_H__

#define RT_NONE		0x00000000
#define RT_LOCALHOST	0x00000001
#define RT_DEFAULT	0x00000002

class RouteEntry
{
public:
	// std::list<PKB*> que;
	unsigned int rt_net;		/* network address */
	unsigned int rt_netmask;	/* mask */
	unsigned int rt_gw;		/* gateway(next-hop) */
	unsigned int rt_flags;		/* route entry flags */
	int rt_metric;			/* distance metric */
	struct netdev *rt_dev;		/* output net device or local net device */

public:
    RouteEntry(/* args */);
    ~RouteEntry();
};





#endif