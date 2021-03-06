#ifndef __RECVROUTE__
#define __RECVROUTE__

#include <stdint.h>

#include <arpa/inet.h>
#include <net/if.h>

struct selfroute {
    uint32_t prefixlen;
    struct in_addr prefix;
    uint32_t ifindex;
    struct in_addr nexthop;
    uint32_t cmdnum;
    char ifname[10];
};

void *receive_rt_change(void *arg);

#endif
