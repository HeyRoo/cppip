/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 14:47:12
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:10:37
 */
#include "vir_eth.h"
#include "tap_if.h"

#include "ether.h"
#include "dbg.h"

#include <string.h>
#include <list>
#include <sys/unistd.h>
#include <string>

extern std::list<struct netdev*> net_devices;

int VirEth::order = 0;

// VirEth::VirEth(/* args */){
// 	/* a veth need to bind a tap */
// 	tap = new TapIF();
// 	dev = new struct netdev();
//     memset(dev, 0, sizeof(struct netdev));
// 	/* set name */
// 	std::string name = "veth" + std::to_string(order); 
// 	dev->net_name[NETDEV_NLEN - 1] = '\0';
// 	strncpy((char *)dev->net_name, (char *)name.c_str(), NETDEV_NLEN - 1);
// 	net_devices.push_back(dev);
// 	order++;
// 	dbg("new veth: ", dev->net_name);
// }

VirEth::VirEth(unsigned int ipaddr, unsigned int net_mask, unsigned char* net_hwaddr){

	tap = new TapIF();

	dev = new struct netdev();
    memset(dev, 0, sizeof(struct netdev));
	/* set name */
	std::string name = "veth" + std::to_string(order); 
	dev->net_name[NETDEV_NLEN - 1] = '\0';
	strncpy((char *)dev->net_name, (char *)name.c_str(), NETDEV_NLEN - 1);
	
	dev->net_mtu = tap->tap_nic->dev.net_mtu;
	dev->net_ipaddr = ipaddr;
	dev->net_mask = net_mask;
    memcpy(dev->net_hwaddr, net_hwaddr, ETH_ALEN);
	net_devices.push_back(dev);
	order++;
	dbg("new veth: ", dev->net_name);
	dbg("ip address", dev->net_ipaddr);
	dbg("hw address", dev->net_hwaddr);
}

VirEth::~VirEth(){
	net_devices.remove(dev);
    delete dev;
	delete tap;
	order--;
}

int VirEth::recv(PKB* pkb){
	int l;
	// TapIF* tap = TapIF::instance();
	l = read(tap->tap_nic->fd, pkb->pk_data, pkb->properties->pk_len);
	
	if (l <= 0) {
		dbg("read net dev");
		dev->net_stats.rx_errors++;
	} else {
		dbg("read net dev size: ", l);
		dev->net_stats.rx_packets++;
		dev->net_stats.rx_bytes += l;
		pkb->properties->pk_len = l;
	}
	return l;
}

int VirEth::xmit(PKB* pkb){
	int l;
	// TapIF* tap = TapIF::instance();
	l = write(tap->tap_nic->fd, pkb->pk_data, pkb->properties->pk_len);
	if (l != pkb->properties->pk_len) {
		dbg("write net dev");
		dev->net_stats.tx_errors++;
	} else {
		dev->net_stats.tx_packets++;
		dev->net_stats.tx_bytes += l;
		dbg("write net dev size: ", l);
	}
	return l;
}