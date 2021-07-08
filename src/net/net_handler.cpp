/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 15:41:55
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-05 00:02:22
 */
#include "net_handler.h"
#include "dbg.h"
#include "lo_eth.h"
#include "ether.h"
#include "arp_parser.h"
// #include "pkb_handler.h"
// #include "tap_if.h"
#include <sys/poll.h>

// extern VirEth* veth;
// extern LoEth* loop;

NetHandler::NetHandler(/* args */){

}
NetHandler::~NetHandler(){
	
}

void NetHandler::net_in(PKB *pkb)
{
	dbg("net_in");
	int ret = pkb->verify_eth();
	if (ret != 0){
		delete pkb;
		return ;
	}
	switch (pkb->properties->pk_pro) {
	case ETH_P_RARP:
//		rarp_in(dev, pkb);
		dbg("RARP-type packet");
		break;
	case ETH_P_ARP:
		// arp_in(dev, pkb);
		// {
		// 	dbg("ARP-type packet");
		// 	ArpParser a(pkb);
		// 	a.arp_recv();
		// }

		dbg("ARP-type packet");
		break;
	case ETH_P_IP:
		// ip_in(dev, pkb);
		dbg("IP-type packet");
		break;
	default:
		dbg("drop unkown-type packet");
		break;
	}
	if(pkb != nullptr){

		delete pkb;
	}
}

void NetHandler::veth_rx(VirEth* veth){
	// PKB * pkb = new PKB(veth->dev->net_mtu);
	PKB * pkb = new PKB(veth->dev);
	if (veth->recv(pkb) > 0)
		net_in(pkb);	/* pass to upper */
	else
		delete pkb;
}

void NetHandler::veth_poll(VirEth* veth)
{
	struct pollfd pfd = {};
	int ret;
	// TapIF* tap = TapIF::instance();
	while (1) {
		pfd.fd = veth->tap->tap_nic->fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		/* one event, infinite time */
		ret = poll(&pfd, 1, -1);
		if (ret <= 0){
			dbg("poll /dev/net/tun");
			exit(1);
		}
		/* get a packet and handle it */
		veth_rx(veth);
	}
}

void NetHandler::netdev_tx(PKB* pkb){
	dbg("send packet");
	delete pkb;
}