/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-12 22:13:55
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 14:38:13
 */
#ifndef __IP_PARSER_H__
#define __IP_PARSER_H__

#include "pkb.h"
#include "ether.h"
#include "ip.h"

class IpParser
{
private:
    PKB* packet;
    struct ether* ehdr;
    struct ip* iphdr;

public:
    IpParser(PKB* pkb);
    ~IpParser();
    void ip_recv();
    void ip_recv_route();
    unsigned short checksum(unsigned short *data, int size, unsigned int origsum);
    unsigned int sum(unsigned short *data, int size, unsigned int origsum);
    void ip_ntoh();
    int is_input();
    int is_output();
    void ip_recv_local();
    void ip_forward();
    void ip_reass();
private:
    unsigned short _htons(unsigned short host){
        return (host >> 8) | ((host << 8) & 0xff00);
    }
};




#endif