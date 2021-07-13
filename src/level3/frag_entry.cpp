/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 13:52:17
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 15:55:33
 */
#include "frag_entry.h"
#include "dbg.h"
#include "ip.h"

FragEntry::	FragEntry(unsigned short id,
	unsigned int src,
	unsigned int dst,
	unsigned short pro,
	unsigned int hlen,
	unsigned int rsize,
	unsigned int size,
	int ttl,
	unsigned int flags){
        frag_ttl = ttl;
        frag_id = id;
        frag_src = src;
        frag_dst = dst;
        frag_pro = pro;
        frag_hlen = hlen;
        frag_size = size;
        frag_rsize = rsize;
        frag_flags = flags;
    }

// FragEntry::FragEntry(/* args */)
// {
// }

FragEntry::~FragEntry()
{
	PKB* pkb = nullptr;
	while (!frag_pkb.empty())
	{
		pkb = frag_pkb.front();
		frag_pkb.pop_front();
		delete pkb;
	}
}

int FragEntry::is_complete()
{
    return frag_flags & FRAG_COMPLETE;
}

int FragEntry::is_full()
{
	return (((frag_flags & FRAG_FL_IN) == FRAG_FL_IN) && (frag_rsize == frag_size));
}

/**
 * @name: 
 * @msg: 判断frag是否完整，判断是否是最后一个碎片，不是则从维护的缓冲队列中，寻找相应顺序的frag插入
 * @param {PKB*} pkb
 * @return {*}
 */
int FragEntry::insert_pkb(PKB* pkb){
	struct ip *iphdr, *fraghdr;
	std::list<PKB*>::iterator it;
	int off, hlen;

	if (is_complete()) {
		dbg("extra fragment for complete reassembled packet");
		return -1;
	}

    iphdr = pkb2ip(pkb);
	off = ipoff(iphdr);
	hlen = iphlen(iphdr);

	/* Is last fragment? */
	if ((iphdr->ip_fragoff & IP_FRAG_MF) == 0) {
		if (frag_flags & FRAG_LAST_IN) {
			dbg("reduplicate last ip fragment");
			return -1;
		}
		frag_flags |= FRAG_LAST_IN;
		frag_size = off + iphdr->ip_len - hlen;
        frag_pkb.push_back(pkb);
        /* 不释放pkb */
        pkb = nullptr;
        frag_rsize += iphdr->ip_len - hlen;
        /* Is full? (If yes, set complete flag) */
        if (is_full())
            frag_flags |= FRAG_COMPLETE;
        return 0;
	}

	/* normal fragment */
	/* find the first fraghdr, in which case fraghdr off < iphdr off */
    for(it = frag_pkb.begin(); it != frag_pkb.end(); it++){
		fraghdr = pkb2ip((*it));
		if (off == ipoff(fraghdr)) {
			dbg("reduplicate ip fragment");
			goto frag_drop;
		}
		if (off > ipoff(fraghdr)) {
			goto frag_found;
		}
    }

    fraghdr = nullptr;

frag_found:
	/* Should strict head check? */
	if (frag_hlen && frag_hlen != hlen) {
		dbg("error ip fragment");
		goto frag_drop;
	} else {
		frag_hlen = hlen;
	}

	/* error: fraghdr off > iphdr off */
	if (fraghdr && ipoff(fraghdr) + fraghdr->ip_len - hlen > off) {
		dbg("error ip fragment");
		goto frag_drop;
	}

	/* Is first one? (And reduplicate ip fragment cannot appear here!) */
	if (off == 0)
		frag_flags |= FRAG_FIRST_IN;

frag_out:
    frag_pkb.insert(++it, pkb);
    pkb = nullptr;
	frag_rsize += iphdr->ip_len - hlen;
	/* Is full? (If yes, set complete flag) */
	if (is_full())
		frag_flags |= FRAG_COMPLETE;
	return 0;

frag_drop:
	return -1;
}

PKB* FragEntry::reass(){
	PKB *pkb, *fragpkb;
	struct ip *fraghdr;
	unsigned char *p;
	int hlen, len;

	pkb = nullptr;
	hlen = frag_hlen;
	len = frag_hlen + frag_size;
	if (len > 65535) {
		dbg("reassembled packet oversize", hlen, len);
		return pkb;
	}

	/* copy ether header and ip header */
	fragpkb = frag_pkb.front();
	fraghdr = pkb2ip(fragpkb);

	pkb = new PKB(len);
	pkb->pk_pro = ETH_P_IP;
	p = pkb->pk_data;
	memcpy(p, fragpkb->pk_data, hlen);

	/* adjacent ip header */
	pkb2ip(pkb)->ip_fragoff = 0;
	pkb2ip(pkb)->ip_len = len;

	p += hlen;

	for(auto it = frag_pkb.begin(); it != frag_pkb.end(); it++){
		fraghdr = pkb2ip((*it));
		memcpy(p, (char *)fraghdr + hlen, fraghdr->ip_len - hlen);
		p += fraghdr->ip_len - hlen;
	}
	dbg("resassembly success", hlen, len);
	return pkb;
}