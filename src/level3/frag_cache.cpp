/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 13:49:28
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 16:02:42
 */
#include "frag_cache.h"

FragCache::FragCache(/* args */)
{
}

FragCache::~FragCache()
{
}

FragCache* FragCache::instance(){
	static FragCache inst;
	return &inst;
}

FragEntry* FragCache::lookup(unsigned short id, unsigned char pro, unsigned int src, unsigned int dst){
    FragEntry* fe = nullptr;
    cache_lock.lock();
    for(auto it = cache.begin(); it != cache.end(); it++){
		if ((*it)->frag_id == id &&
			(*it)->frag_pro == pro &&
			(*it)->frag_src == src &&
			(*it)->frag_dst == dst){
                fe = (*it);    
            }
    }
    cache_lock.unlock();
    return fe;
}

/**
 * @name: 
 * @msg: 分配一个 frag entry
 * @param {unsigned short} id
 * @param {unsigned char} pro
 * @param {unsigned int} src
 * @param {unsigned int} dst
 * @return {*} 返回的 entry 已经存在于 cache 中
 */
FragEntry* FragCache::alloc_entry(unsigned short id, unsigned char pro, unsigned int src, unsigned int dst){
	FragEntry *frag = new FragEntry(id, src, dst, pro, 0, 0, 0, FRAG_TIME, 0);
    
    cache_lock.lock();
	cache.push_front(frag);
    cache_lock.unlock();
	return frag;
}

void FragCache::remove(FragEntry* frag){
    cache_lock.lock();
    cache.remove(frag);
    cache_lock.unlock();
    delete frag;
}

// int FragCache::insert_frag(PKB* pkb, FragEntry* frag){
// 	struct pkbuf *fragpkb;
// 	struct ip *iphdr, *fraghdr;
// 	struct list_head *pos;
// 	int off, hlen;

// 	if (complete_frag(frag)) {
// 		ipdbg("extra fragment for complete reassembled packet");
// 		goto frag_drop;
// 	}

// 	iphdr = pkb2ip(pkb);
// 	off = ipoff(iphdr);
// 	hlen = iphlen(iphdr);

// 	/* Is last fragment? */
// 	if ((iphdr->ip_fragoff & IP_FRAG_MF) == 0) {
// 		if (frag->frag_flags & FRAG_LAST_IN) {
// 			ipdbg("reduplicate last ip fragment");
// 			goto frag_drop;
// 		}
// 		frag->frag_flags |= FRAG_LAST_IN;
// 		frag->frag_size = off + iphdr->ip_len - hlen;
// 		pos = frag->frag_pkb.prev;
// 		goto frag_out;
// 	}

// 	/* normal fragment */
// 	pos = &frag->frag_pkb;
// 	/* find the first fraghdr, in which case fraghdr off < iphdr off */
// 	list_for_each_entry_reverse(fragpkb, &frag->frag_pkb, pk_list) {
// 		fraghdr = pkb2ip(fragpkb);
// 		if (off == ipoff(fraghdr)) {
// 			ipdbg("reduplicate ip fragment");
// 			goto frag_drop;
// 		}
// 		if (off > ipoff(fraghdr)) {
// 			pos = &fragpkb->pk_list;
// 			goto frag_found;
// 		}
// 	}
// 	/* not found: pkb is the current min-offset fragment */
// 	fraghdr = NULL;

// frag_found:
// 	/* Should strict head check? */
// 	if (frag->frag_hlen && frag->frag_hlen != hlen) {
// 		ipdbg("error ip fragment");
// 		goto frag_drop;
// 	} else {
// 		frag->frag_hlen = hlen;
// 	}

// 	/* error: fraghdr off > iphdr off */
// 	if (fraghdr && ipoff(fraghdr) + fraghdr->ip_len - hlen > off) {
// 		ipdbg("error ip fragment");
// 		goto frag_drop;
// 	}

// 	/* Is first one? (And reduplicate ip fragment cannot appear here!) */
// 	if (off == 0)
// 		frag->frag_flags |= FRAG_FIRST_IN;

// frag_out:
// 	list_add(&pkb->pk_list, pos);
// 	frag->frag_rsize += iphdr->ip_len - hlen;
// 	/* Is full? (If yes, set complete flag) */
// 	if (full_frag(frag))
// 		frag->frag_flags |= FRAG_COMPLETE;
// 	return 0;

// frag_drop:
// 	free_pkb(pkb);
// 	return -1;
// }