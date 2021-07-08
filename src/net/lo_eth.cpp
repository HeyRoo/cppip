/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 14:47:12
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:24:18
 */
#include "lo_eth.h"
#include "net_config.h"
#include "dbg.h"
#include "net_handler.h"

#include <string.h>
#include <list>

extern std::list<struct netdev*> net_devices;

LoEth::LoEth(unsigned int mtu, unsigned int ipaddr, unsigned int net_mask){
	dev = new struct netdev();
    memset(dev, 0, sizeof(struct netdev));
	/* set name */
	dev->net_name[NETDEV_NLEN - 1] = '\0';
	char * name = (char *)"lo";
	strncpy((char *)dev->net_name, name, NETDEV_NLEN - 1);
	dev->net_mtu = mtu;
	dev->net_ipaddr = ipaddr;
	dev->net_mask = net_mask;
	net_devices.push_back(dev);
}

LoEth::~LoEth(){
	net_devices.remove(dev);	
    delete dev;
}

// void LoEth::init(){
// 	/* init veth: information for our netstack */
// 	dev->net_mtu = LOOPBACK_MTU;
// 	dev->net_ipaddr = LOOPBACK_IPADDR;
// 	dev->net_mask = LOOPBACK_NETMASK;
// 	dbg(dev->net_name, "ip address", dev->net_ipaddr, "hw address", dev->net_hwaddr);
// }

int LoEth::recv(PKB *pkb){
	dev->net_stats.rx_packets++;
	dev->net_stats.rx_bytes += pkb->properties->pk_len;
	return 0;
}

int LoEth::xmit(PKB *pkb){
	/* get pkb */
	pkb->properties->pk_refcnt++;
	/* loop back to itself */
	recv(pkb);
	dev->net_stats.tx_packets++;
	dev->net_stats.tx_bytes += pkb->properties->pk_len;
	return pkb->properties->pk_len;
}