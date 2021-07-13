/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-02 17:26:21
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 19:45:59
 */
#include "arp_parser.h"
#include "dbg.h"
#include "ip.h"
#include "arp_cache.h"
#include "net_handler.h"

ArpParser::ArpParser(PKB* pkb)
{
    packet = pkb; 
    ehdr = (struct ether *)pkb->pk_data;
    ahdr = (struct arp *)ehdr->eth_data;
}

ArpParser::~ArpParser()
{
}

int ArpParser::parse(){
    if (packet->properties->pk_type == PKT_OTHERHOST) {
		dbg("arp(l2) packet is not for us");
		return 1;
	}

	if (packet->properties->pk_len < ARP_HRD_SZ) {
		dbg("arp packet is too small");
		return 1;
	}

	/* safe check for arp cheating */
	if (memcmp(ahdr->arp_sha, ehdr->eth_src, ETH_ALEN) != 0) {
		dbg("error sender hardware address");
		return 1;
	}
	arp_hton();

#if defined(ARP_ETHERNET) && defined(ARP_IP)
	/* ethernet/ip arp only */
	if (ahdr->arp_hrd != ARP_HRD_ETHER || ahdr->arp_pro != ETH_P_IP ||
		ahdr->arp_hrdlen != ETH_ALEN || ahdr->arp_prolen != IP_ALEN) {
		dbg("unsupported L2/L3 protocol");
		return 1;
	}
#endif
	if (ahdr->arp_op != ARP_OP_REQUEST && ahdr->arp_op != ARP_OP_REPLY) {
		dbg("unknown arp operation");
		return 1;
	}

	arp_recv();
	return 0;
}

void ArpParser::arp_hton(){
	ahdr->arp_hrd = _htons(ahdr->arp_hrd);
	ahdr->arp_pro = _htons(ahdr->arp_pro);
	ahdr->arp_op = _htons(ahdr->arp_op);
}

void ArpParser::arp_recv(){
	if (MULTICAST(ahdr->arp_tip)) {
		dbg("multicast tip");
		return ;
	}

	if (ahdr->arp_tip != packet->properties->pk_indev->net_ipaddr) {
		dbg("not for us");
		return ;
	}
    // printf("arp processing\n");
	ArpEntry* ae;
	ArpCache* cache = ArpCache::instance();
    dbg("arp processing");
	ae = cache->lookup(ahdr->arp_pro, ahdr->arp_sip);
	if (ae) {
		/* passive learning(REQUEST): update old arp entry in cache */
        memcpy(ae->ae_hwaddr, ahdr->arp_sha, ETH_ALEN);
		/* send waiting packet (maybe we receive arp reply) */
		if (ae->ae_state == ARP_WAITING)
			ae->send_queue();
		ae->ae_state = ARP_RESOLVED;
		ae->ae_ttl = ARP_TIMEOUT;
	} else if (ahdr->arp_op == ARP_OP_REQUEST) {
		/* Unsolicited ARP reply is not accepted */
		cache->insert(packet->properties->pk_indev, ahdr->arp_pro, ahdr->arp_sip, ahdr->arp_sha);
	}

	if (ahdr->arp_op == ARP_OP_REQUEST) {
		arp_reply();
	}
}

void ArpParser::arp_reply(){
	dbg("replying arp request");
	/* arp field */
	ahdr->arp_op = ARP_OP_REPLY;
	memcpy(ahdr->arp_tha, ahdr->arp_sha, ETH_ALEN);
	ahdr->arp_tip = ahdr->arp_sip;
	memcpy(ahdr->arp_sha, packet->properties->pk_indev->net_hwaddr, ETH_ALEN);
	ahdr->arp_sip = packet->properties->pk_indev->net_ipaddr;
	arp_hton();
	/* ether field */
	// netdev_tx(dev, pkb, ARP_HRD_SZ, ETH_P_ARP, ehdr->eth_src);
	NetHandler::netdev_tx(packet);
}

