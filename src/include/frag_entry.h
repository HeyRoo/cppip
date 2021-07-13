/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 13:51:32
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 15:40:40
 */
#ifndef __FRAG_ENTRY_H__
#define __FRAG_ENTRY_H__

#include "pkb.h"
#include <list>

#define FRAG_COMPLETE	0x00000001
#define FRAG_FIRST_IN	0x00000002
#define FRAG_LAST_IN	0x00000004
#define FRAG_FL_IN	0x00000006	/* first and last in*/

#define FRAG_TIME	30	/* exceed defragment time: 30 sec */

class FragEntry
{
public:
	unsigned short frag_id;
	unsigned int frag_src;
	unsigned int frag_dst;
	unsigned short frag_pro;
	unsigned int frag_hlen;
	unsigned int frag_rsize;/* size of received fragments */
	unsigned int frag_size;	/* total fragments payload size(not contain ip header) */
	int frag_ttl;		/* reassembly timer */
	unsigned int frag_flags;/* fragment flags */

    std::list<PKB*> frag_pkb;
public:
	int is_complete();
	int is_full();

public:
	FragEntry(unsigned short id,
	unsigned int src,
	unsigned int dst,
	unsigned short pro,
	unsigned int hlen,
	unsigned int rsize,
	unsigned int size,
	int ttl,
	unsigned int flags);
    // FragEntry(/* args */);
    ~FragEntry();
	PKB* reass();
	int insert_pkb(PKB* pkb);
	
};


#endif