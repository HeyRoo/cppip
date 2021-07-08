/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-28 16:25:14
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-07-05 00:04:18
 */
// #define DBG_MACRO_NO_WARNING
// #define DBG_MACRO_DISABLE

#include <iostream>
#include <thread>

#include "tap_if.h"
#include "vir_eth.h"
#include "lo_eth.h"
#include "net_handler.h"

LoEth* loop;
VirEth* veth;

void net_stack_init(){
    loop = new LoEth();

    veth = new VirEth();
}

void run(){
    std::thread t1(NetHandler::veth_poll, veth);
    t1.join();
}

int main(int argc, char const *argv[])
{
    net_stack_init();
    // a->tap_init();
    // job_start();
    run();
    // delete loop;
    // delete veth;
    return 0;
}
