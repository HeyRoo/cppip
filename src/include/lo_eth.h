/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 15:27:03
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:22:32
 */
#ifndef __LO_ETH_H__
#define __LO_ETH_H__

#include "net_if.h"
#include "pkb.h"
#include "net_config.h"

class LoEth
{
private:
    /* data */
    struct netdev* dev;
public:
    LoEth(unsigned int mtu = LOOPBACK_MTU, unsigned int ipaddr = LOOPBACK_IPADDR, unsigned int net_mask = LOOPBACK_NETMASK);
    ~LoEth();
    // void init();
    int recv(PKB *pkb);
    int xmit(PKB *pkb);
};

#endif