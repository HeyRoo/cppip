/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-04 11:36:45
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-04 11:41:39
 */
#ifndef __IP_H__
#define __IP_H__

/* IP Packet Format */
#define IP_ALEN 4

#define MULTICAST(netip) ((0x000000f0 & (netip)) == 0x000000e0)

#endif