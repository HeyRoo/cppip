/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-01 20:24:47
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-05 00:01:16
 */
#include "pkb.h"
#include "dbg.h"
#include "ether.h"
#include <string.h>

int PKB::alloc_pkbs = 0;
int PKB::free_pkbs = 0;

PKB::PKB(int len)
{
	properties = new struct pkhdr();
	pk_data = new unsigned char[len];
	properties->pk_len = len;
	properties->pk_pro = 0xffff;
	properties->pk_type = 0;
	properties->pk_refcnt = 1;
	properties->pk_indev = nullptr;
	properties->pk_rtdst = nullptr;
	alloc_pkbs++;
	_is_safe();
}

PKB::PKB(const struct netdev* dev)
{
	properties = new struct pkhdr();
	pk_data = new unsigned char[dev->net_mtu];
	properties->pk_len = dev->net_mtu;
	properties->pk_pro = 0xffff;
	properties->pk_type = 0;
	properties->pk_refcnt = 1;
	properties->pk_indev = (struct netdev* )dev;
	properties->pk_rtdst = nullptr;
	alloc_pkbs++;
	_is_safe();
}


PKB::PKB(const PKB* obj)
{
	properties = new struct pkhdr();
	properties->pk_len = obj->properties->pk_len;
	properties->pk_pro = obj->properties->pk_pro;
	properties->pk_type = obj->properties->pk_type;
	properties->pk_refcnt = obj->properties->pk_refcnt;
	properties->pk_indev = obj->properties->pk_indev;
	properties->pk_rtdst = obj->properties->pk_rtdst;
    pk_data = new unsigned char[obj->properties->pk_len];
    memcpy(pk_data, obj->pk_data, obj->properties->pk_len);
	alloc_pkbs++;
	_is_safe();
}

PKB::~PKB()
{
    delete []pk_data;
    delete properties;
	free_pkbs++;
}

void PKB::_is_safe(){
    if ((alloc_pkbs - free_pkbs) > MAX_PKBS) {
		dbg(alloc_pkbs, free_pkbs);
		dbg("oops: too many pkbuf");
		exit(1);
	}
}

void PKB::trim(int len){
    if(properties->pk_len < len){
        return ;
    }
    unsigned char * rdata = new unsigned char[len];
    memcpy(rdata, pk_data, len);
    properties->pk_len = len;
    delete [] pk_data;
    pk_data = rdata;
    dbg("trim pkb :", len);
}

void PKB::get_pkb(){
    properties->pk_refcnt++;
}

int PKB::verify_eth(){
	struct ether *ehdr = (struct ether *)pk_data;
	if (properties->pk_len < ETH_HRD_SZ) {
		dbg("received packet is too small!");
		return 1;
	}
	/* hardware address type */
	if (_is_eth_multicast(ehdr->eth_dst)) {
		if (_is_eth_broadcast(ehdr->eth_dst))
			properties->pk_type = PKT_BROADCAST;
		else
			properties->pk_type = PKT_MULTICAST;
	} else if (!memcmp(ehdr->eth_dst, properties->pk_indev->net_hwaddr, ETH_ALEN)) {
			properties->pk_type = PKT_LOCALHOST;
	} else {
			properties->pk_type = PKT_OTHERHOST;
	}
	/* packet protocol */
	properties->pk_pro = _htons(ehdr->eth_pro);
	return 0;
}

int PKB::_is_eth_multicast(unsigned char *hwa)
{
	return (hwa[0] & 0x01);
}

int PKB::_is_eth_broadcast(unsigned char *hwa)
{
	/*
	 * fast compare method
	 * ethernet mac broadcast is FF:FF:FF:FF:FF:FF
	 */
	return (hwa[0] & hwa[1] & hwa[2] & hwa[3] & hwa[4] & hwa[5]) == 0xff;
}

unsigned short PKB::_htons(unsigned short host)
{
	return (host >> 8) | ((host << 8) & 0xff00);
}