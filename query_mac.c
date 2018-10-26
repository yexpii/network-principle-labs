#include "query_mac.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

int arp_get_mac(macaddr_t mac, char *if_name, char *ip_str) { 

    struct arpreq arp_req;
    struct sockaddr_in *sin;

    sin = (struct sockaddr_in *)&(arp_req.arp_pa);

    memset(&arp_req, 0, sizeof(arp_req));
    sin->sin_family = AF_INET;
    inet_pton(AF_INET, ip_str, &(sin->sin_addr));
    strncpy(arp_req.arp_dev, if_name, IF_NAMESIZE - 1);

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);

    int ret = ioctl(sfd, SIOCGARP, &arp_req);
    if (ret < 0) {
        fprintf(stderr, "Get ARP entry failed : %s\n", strerror(errno));
        exit(2);
    }

    if (arp_req.arp_flags & ATF_COM) {
        memcpy(mac, (unsigned char *)arp_req.arp_ha.sa_data, sizeof(macaddr_t));
        return 0;
    } else {
        return 1;
    }
    
}

int if_get_mac(macaddr_t mac, char *ifname) {

    struct ifreq ifr;
 
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    strncpy(ifr.ifr_name, ifname, IF_NAMESIZE - 1);
    int ret = ioctl(sfd, SIOCGIFHWADDR, &ifr);

    if (ret < 0) {
        fprintf(stderr, "Get MAC address failed : %s\n", strerror(errno));
        exit(2);
    }

    memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    return 0;
}