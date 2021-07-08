/*
 * @Author: your name
 * @Date: 2021-06-28 16:26:43
 * @LastEditTime: 2021-07-01 22:53:54
 * @LastEditors: HeyRoo
 * @Description: In User Settings Edit
 * @FilePath: /cppip/src/include/tap_if.h
 */
#ifndef __TAP_IF_H__
#define __TAP_IF_H__

#include "net_if.h"
#include "net_config.h"

#define TUNTAPDEV "/dev/net/tun"

struct tapdev {
	struct netdev dev;
	int fd;
};

class TapIF
{
public:
    tapdev * tap_nic;
    int sock_fd = -1;
    static int order;

public:
    // static TapIF* instance();
    TapIF(unsigned int ipaddr = FAKE_TAP_ADDR, unsigned int net_mask = FAKE_TAP_NETMASK);
    // TapIF(/* args */);
    ~TapIF();
    
    void tap_delete();
    
    void get_hwaddr();
    void get_name();
    void get_ipaddr();
    void set_ipaddr(unsigned int ipaddr);
    void get_mtu();
    void set_up();
    void set_down();
    void set_flags(unsigned short flags, int set);
    void set_netmask(unsigned int netmask);
    int set_persist();
    void set_mod();
    void unset_mod();

    void _tap_free();
    int _tap_alloc(char* name);

private:

    void _tap_init(unsigned int ipaddr, unsigned int net_mask);

};

#endif