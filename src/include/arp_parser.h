/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:08:33
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 18:56:30
 */
#ifndef __ARP_PARSER_H__
#define __ARP_PARSER_H__

#include "pkb.h"
#include "ether.h"
#include "arp.h"


class ArpParser
{
private:
    PKB* packet;
    struct ether* ehdr;
    struct arp* ahdr;
public:
    ArpParser(PKB* pkb);
    ~ArpParser();

    int parse();
    void arp_recv();
    void arp_hton();
    void arp_reply();

    

private:
    unsigned short _htons(unsigned short host){
        return (host >> 8) | ((host << 8) & 0xff00);
    }
};






#endif