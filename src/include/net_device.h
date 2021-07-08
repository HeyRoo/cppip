/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 14:39:13
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-01 23:12:33
 */
#ifndef __NET_DEVICE_H__
#define __NET_DEVICE_H__

#include "net_if.h"
#include "pkb.h"

class NetDevice
{
public:
    /* data */
    struct netdev* dev;

public:
    
    NetDevice(/* args */);
    virtual ~NetDevice() = 0;
    virtual void init() = 0;
    virtual int recv(PKB* pkb) = 0;
    virtual int xmit(PKB* pkb) = 0;
    // virtual void exit() = 0;
    // struct netdev * alloc(char *name);
    
private:
};



#endif