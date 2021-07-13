/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-07-13 13:49:40
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-13 15:59:19
 */
#ifndef __FRAG_CACHE_H__
#define __FRAG_CACHE_H__

#include "frag_entry.h"
// #include "pkb.h"
#include <mutex>
#include <list>

class FragCache
{
private:
    std::list<FragEntry*> cache;
	std::mutex cache_lock;
public:
	static FragCache* instance();
	FragEntry* lookup(unsigned short id, unsigned char pro, unsigned int src, unsigned int dst);
	// int insert(unsigned short id, unsigned char pro, unsigned int src, unsigned int dst);
	FragEntry* alloc_entry(unsigned short id, unsigned char pro, unsigned int src, unsigned int dst);
    void remove(FragEntry* frag);
    
private:
    FragCache(/* args */);
    ~FragCache();
};




#endif