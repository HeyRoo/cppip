/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 11:55:59
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 12:02:07
 */
#ifndef __ICMP_HANDLER_H__
#define __ICMP_HANDLER_H__

#include "pkb.h"
#include "icmp.h"

class ICMPHandler
{
private:
    /* data */
public:
    ICMPHandler(PKB* pkb);
    ~ICMPHandler();

    void send(unsigned char type, unsigned char code, unsigned int data);
};


#endif