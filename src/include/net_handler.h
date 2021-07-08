/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 15:42:24
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 23:46:31
 */
#ifndef __NET_HANDLER_H__
#define __NET_HANDLER_H__

#include "vir_eth.h"
#include "pkb.h"

class NetHandler
{
private:
    /* data */
public:
    NetHandler(/* args */);
    ~NetHandler();
    static void net_in(PKB *pkb);
    static void veth_rx(VirEth *veth);
    static void veth_poll(VirEth *veth);
    static void netdev_tx(PKB* pkb);
    // static void lo_rx();
};

#endif