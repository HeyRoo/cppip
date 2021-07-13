/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 14:47:26
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:16:11
 */
#ifndef __VIR_ETH_H__
#define __VIR_ETH_H__

#include "net_config.h"
#include "pkb.h"
#include "tap_if.h"

class VirEth
{
private:
    static int order;

public:
    TapIF* tap;
    struct netdev* dev;
public:
    // VirEth(/* args */);
    VirEth(unsigned int ipaddr = FAKE_IPADDR, unsigned int net_mask = FAKE_NETMASK, unsigned char* net_hwaddr = (unsigned char *)FAKE_HWADDR);
    ~VirEth();
    void init();
    int recv(PKB* pkb);
    int xmit(PKB* pkb);
};

#endif