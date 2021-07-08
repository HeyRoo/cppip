/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 13:55:21
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:12:12
 */
#include "net_if.h"
#include "net_device.h"
#include <list>
#include <stdlib.h>
#include <string.h>

std::list<struct netdev*> net_devices;

NetDevice::NetDevice(/* args */)
{
}

NetDevice::~NetDevice()
{
}

// struct netdev * NetDevice::alloc(char *name){
// 	struct netdev *dev;
// 	dev = new struct netdev();
//     memset(dev, 0, sizeof(struct netdev));
// 	/* add into localhost net device list */
// 	net_devices.push_back(dev);
// 	/* set name */
// 	dev->net_name[NETDEV_NLEN - 1] = '\0';
// 	strncpy((char *)dev->net_name, name, NETDEV_NLEN - 1);
// 	return dev;
// }