/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-01 20:24:56
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 14:45:19
 */
#ifndef __PKB_H__
#define __PKB_H__

#include "net_if.h"

#define MAX_PKBS 200

class PKB
{
public:
    static int free_pkbs;
    static int alloc_pkbs;
    struct pkhdr* properties;
    unsigned char* pk_data; 

public:
    PKB(int len);
    PKB(const struct netdev* dev);
    PKB(const PKB* obj);
    ~PKB();
    void trim(int len);
    void get_pkb();
    int verify_eth();

private:
    void _is_safe();
    int _is_eth_multicast(unsigned char *hwa);
    int _is_eth_broadcast(unsigned char *hwa);
    unsigned short _htons(unsigned short host);
};




#endif