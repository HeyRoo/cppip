/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 12:01:19
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 12:01:51
 */
#ifndef __ICMP_H__
#define __ICMP_H__

/* icmp type */
#define ICMP_T_ECHORLY		0
#define ICMP_T_DUMMY_1		1
#define ICMP_T_DUMMY_2		2
#define ICMP_T_DESTUNREACH	3
#define ICMP_T_SOURCEQUENCH	4
#define ICMP_T_REDIRECT		5
#define ICMP_T_DUMMY_6		6
#define ICMP_T_DUMMY_7		7
#define ICMP_T_ECHOREQ		8
#define ICMP_T_DUMMY_9		9
#define ICMP_T_DUMMY_10		10
#define ICMP_T_TIMEEXCEED	11
#define ICMP_T_PARAMPROBLEM	12	/* parameter problem */
#define ICMP_T_TIMESTAMPREQ	13
#define ICMP_T_TIMESTAMPRLY	14
#define ICMP_T_INFOREQ		15
#define ICMP_T_INFORLY		16
#define ICMP_T_ADDRMASKREQ	17
#define ICMP_T_ADDRMASKRLY	18
#define ICMP_T_MAXNUM		18

/* icmp code of ICMP_T_DESTUNREACH type */
#define ICMP_NET_UNREACH	0
#define ICMP_HOST_UNREACH	1
#define ICMP_PROTO_UNREACH	2
#define ICMP_PORT_UNREACH	3
#define ICMP_FRAG_NEEDED	4
#define ICMP_SROUTE_FAILED	5	/* source route: according to ip option */
#define ICMP_NET_UNKNOWN	6	/* RFC 1812: should use code 0 */
#define ICMP_HOST_UNKNOWN	7
#define ICMP_SHOST_ISOLATED	8
#define ICMP_NET_ADMINPRO	9
#define ICMP_HOST_ADMINPRO	10
#define ICMP_NET_UNREACHTOS	11
#define ICMP_HOST_UNREACHTOS	12
#define ICMP_ADMINPRO		13	/* administratively prohibited */
#define ICMP_PREC_VIOLATION	14	/* host precedence violation */
#define ICMP_PREC_CUTOFF	15	/* precedence cutoff in effect */

/* icmp code of ICMP_T_REDIRECT type */
#define ICMP_REDIRECT_NET	0	/* net, obsolete(RFC 1812-5.2.7.2) */
#define ICMP_REDIRECT_HOST	1	/* host */
#define ICMP_REDIRECT_TOSNET	2	/* type of service and net ,obsolete */
#define ICMP_REDIRECT_TOSHOST	3	/* type of service and host */

/* icmp code of ICMP_T_TIMEEXCEED type */
#define ICMP_EXC_TTL		0
#define ICMP_EXC_FRAGTIME	1


#endif