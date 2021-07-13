/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 22:12:22
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 16:05:54
 */
#include "ip_parser.h"
#include "dbg.h"
#include "route_cache.h"
#include "icmp_handler.h"
#include "frag_cache.h"

IpParser::IpParser(PKB* pkb)
{
    packet = pkb;
    ehdr = (struct ether*)pkb->pk_data;
    iphdr = (struct ip*)ehdr->eth_data;
}

IpParser::~IpParser()
{
}

void IpParser::ip_recv(){
	int hlen;

	/* Fussy sanity check */
	if (packet->properties->pk_type == PKT_OTHERHOST) {
		dbg("ip(l2) packet is not for us");
		return;
	}

	if (packet->properties->pk_len < IP_HRD_SZ) {
		dbg("ip packet is too small");
		return;
	}

	if (ipver(iphdr) != IP_VERSION_4) {
		dbg("ip packet is not version 4");
		return;
	}

	hlen = iphlen(iphdr);
	if (hlen < IP_HRD_SZ) {
		dbg("ip header is too small");
		return;
	}

	if (checksum((unsigned short *)iphdr, hlen, 0) != 0) {
		dbg("ip checksum is error");
		return;
	}

	ip_ntoh();
	if (iphdr->ip_len < hlen ||
		packet->properties->pk_len <  iphdr->ip_len) {
		dbg("ip size is unknown");
		return;
	}

	if (packet->properties->pk_len >  iphdr->ip_len){
        packet->trim(iphdr->ip_len);
    }

	/* Now, we can take care of the main ip processing safely. */
	ip_recv_route();
	return;

}

int IpParser::is_input(){
	RouteCache* cache = RouteCache::instance();
	RouteEntry* re = cache->lookup(iphdr->ip_dst);
	if(!re){
		/* TODO */
		ip_ntoh();
		ICMPHandler ih(packet);
		ih.send(ICMP_T_DESTUNREACH, ICMP_NET_UNREACH, 0);
		return -1;
	}
	packet->properties->pk_rtdst = re;
	return 0;
}

int IpParser::is_output(){
	RouteCache* cache = RouteCache::instance();
	RouteEntry* re = cache->lookup(iphdr->ip_dst);
	if (!re) {
		/* FIXME: icmp dest unreachable to localhost */
		dbg("No route entry to ");
		return -1;
	}
	packet->properties->pk_rtdst = re;
	iphdr->ip_src = re->rt_dev->net_ipaddr;
	dbg("Find route entry from  to ");
	return 0;
}

void IpParser::ip_recv_route(){
	
	if (is_input() < 0)
		return;
	/* Is this packet sent to us? */
	if (packet->properties->pk_rtdst->rt_flags & RT_LOCALHOST) {
		ip_recv_local();
	} else {
		ip_ntoh();
		ip_forward();
	}
}

void IpParser::ip_reass(){
	FragEntry *frag = nullptr;
	FragCache *cache = FragCache::instance();
	
	frag = cache->lookup(iphdr->ip_id, iphdr->ip_pro, iphdr->ip_src, iphdr->ip_dst);

	if (frag == nullptr)
		frag = cache->alloc_entry(iphdr->ip_id, iphdr->ip_pro, iphdr->ip_src, iphdr->ip_dst);
	if (frag->insert_pkb(packet) < 0)
		return ;
	if (frag->is_complete()){
		packet = frag->reass();
		cache->remove(frag);
	}else{
		packet = nullptr;
	}
}

void IpParser::ip_recv_local(){
	/* fragment reassambly */
	if (iphdr->ip_fragoff & (IP_FRAG_OFF | IP_FRAG_MF)) {
		if (iphdr->ip_fragoff & IP_FRAG_DF) {
			dbg("error fragment");
			// free_pkb(pkb);
			return;
		}
		ip_reass();
		/* pkb到此处首先进缓冲队列，如果datagram完整，则返回新的pkg继续处理，没有则为空，跳出函数 */
		if (!packet)
			return;
		iphdr = pkb2ip(packet);
	}

	/* copy pkb to raw */
	// raw_in(packet);

	/* pass to upper-level */
	switch (iphdr->ip_pro) {
	case IP_P_ICMP:
		// icmp_in(packet);
		break;
	case IP_P_TCP:
		// tcp_in(packet);
		break;
	case IP_P_UDP:
		// udp_in(packet);
		break;
	default:
		// free_pkb(packet);
		// dbg("unknown protocol");
		break;
	}
}

void IpParser::ip_forward(){

}

unsigned short IpParser::checksum(unsigned short *data, int size, unsigned int origsum)
{
	origsum = sum(data, size, origsum);
	origsum = (origsum & 0xffff) + (origsum >> 16);
	origsum = (origsum & 0xffff) + (origsum >> 16);
	return (~origsum & 0xffff);
}

unsigned int IpParser::sum(unsigned short *data, int size, unsigned int origsum)
{
	while (size > 1) {
		origsum += *data++;
		size -= 2;
	}
	if (size)
		origsum += _htons(((*(unsigned char *)data) & 0xff) << 8);
	return origsum;
}

void IpParser::ip_ntoh()
{
        iphdr->ip_len = _htons(iphdr->ip_len);
        iphdr->ip_id = _htons(iphdr->ip_id);
        iphdr->ip_fragoff = _htons(iphdr->ip_fragoff);
}