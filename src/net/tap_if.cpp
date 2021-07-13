#include "tap_if.h"
#include "dbg.h"

#include "ether.h"

#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>

int TapIF::order = 0;

// TapIF::TapIF(/* args */)
// {
//     tap_nic = new tapdev();
// 	memset(tap_nic, 0, sizeof(struct tapdev));
// }

TapIF::TapIF(unsigned int ipaddr, unsigned int net_mask)
{
    tap_nic = new tapdev();
	memset(tap_nic, 0, sizeof(struct tapdev));
	_tap_init(ipaddr, net_mask);
}

TapIF::~TapIF()
{
	dbg("if close");
	// int ret = close(tap_nic->fd);
	// dbg("close ret ", ret);
	tap_delete();
    _tap_free();
	order--;
}

// TapIF* TapIF::instance(){
//     static TapIF inst(FAKE_TAP_ADDR, FAKE_TAP_NETMASK);
//     return &inst;
// }

void TapIF::_tap_init(unsigned int ipaddr, unsigned int net_mask)
{
	std::string name = "tap" + std::to_string(order); 
	
	tap_nic->fd = _tap_alloc((char *)name.c_str());
	if (tap_nic->fd < 0){
		dbg("fail to creat tap", tap_nic->fd);
		exit(1);
	}
		// goto free_tap;
	if (set_persist() < 0){
		dbg("fail to set persist");
		exit(1);
	}
		// goto close_tap;
	/* set tap information */
	set_mod();
	get_name();
	get_mtu();

	/* TOPO */
	get_hwaddr();
	set_ipaddr(ipaddr);
	get_ipaddr();
	set_netmask(net_mask);
	set_up();
	// set_netmask(FAKE_TAP_NETMASK);
	unset_mod();
	
	order++;
	return ;

// close_tap:
// 	close(tap_nic->fd);
// free_tap:
// 	_tap_free();
// 	return ;
}

void TapIF::tap_delete(){
	if (ioctl(tap_nic->fd, TUNSETPERSIST, 0) < 0)
		return;
	close(tap_nic->fd);
}


void TapIF::get_hwaddr(){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));
	/* get net order hardware address */
	if (ioctl(tap_nic->fd, SIOCGIFHWADDR, (void *)&ifr) < 0){
		dbg("ioctl SIOCGIFHWADDR");
		exit(1);
	}
	memcpy(tap_nic->dev.net_hwaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
	dbg("mac addr: ", tap_nic->dev.net_hwaddr);
}

void TapIF::get_name(){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));
	if (ioctl(tap_nic->fd, TUNGETIFF, (void *)&ifr) < 0){
		dbg("ioctl SIOCGIFHWADDR");
		exit(1);
	}
	strcpy((char *)tap_nic->dev.net_name, ifr.ifr_name);
	dbg("net device: ", tap_nic->dev.net_name);
}

void TapIF::get_ipaddr(){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));
	struct sockaddr_in *saddr;

	strcpy(ifr.ifr_name, (char *)tap_nic->dev.net_name);
	if (ioctl(sock_fd, SIOCGIFADDR, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("socket SIOCGIFADDR");
		exit(1);
	}
	saddr = (struct sockaddr_in *)&ifr.ifr_addr;
	tap_nic->dev.net_ipaddr = saddr->sin_addr.s_addr;
	dbg("get IPaddr: ", tap_nic->dev.net_ipaddr);
}

void TapIF::set_ipaddr(unsigned int ipaddr){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));
	struct sockaddr_in *saddr;

	strcpy(ifr.ifr_name, (char *)tap_nic->dev.net_name);
	saddr = (struct sockaddr_in *)&ifr.ifr_addr;
	saddr->sin_family = AF_INET;
	saddr->sin_addr.s_addr = ipaddr;
	if (ioctl(sock_fd, SIOCSIFADDR, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("socket SIOCSIFADDR");
		exit(1);
	}
	dbg("set IPaddr: ", tap_nic->dev.net_ipaddr);
}

void TapIF::get_mtu(){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));

	strcpy(ifr.ifr_name, (char *)tap_nic->dev.net_name);
	/* get net order hardware address */
	if (ioctl(sock_fd, SIOCGIFMTU, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("ioctl SIOCGIFHWADDR");
		exit(1);
	}
	tap_nic->dev.net_mtu = ifr.ifr_mtu;
	dbg("mtu: ", tap_nic->dev.net_mtu);
}

void TapIF::set_up(){
	set_flags(IFF_UP | IFF_RUNNING, 1);
	dbg("ifup ", tap_nic->dev.net_name);
}

void TapIF::set_flags(unsigned short flags, int set){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));

	strcpy(ifr.ifr_name, (char *)tap_nic->dev.net_name);
	/* get original flags */
	if (ioctl(sock_fd, SIOCGIFFLAGS, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("socket SIOCGIFFLAGS");
		exit(1);
	}
	/* set new flags */
	if (set)
		ifr.ifr_flags |= flags;
	else
		ifr.ifr_flags &= ~flags & 0xffff;
	if (ioctl(sock_fd, SIOCSIFFLAGS, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("socket SIOCGIFFLAGS");
		exit(1);
	}
}

void TapIF::set_down(){
	set_flags(IFF_UP | IFF_RUNNING, 0);
	dbg("ifdown ", tap_nic->dev.net_name);
}

void TapIF::set_netmask(unsigned int netmask){
	struct ifreq ifr;
	memset(&ifr, 0x0, sizeof(ifr));
	struct sockaddr_in *saddr;

	strcpy(ifr.ifr_name, (char *)tap_nic->dev.net_name);
	saddr = (struct sockaddr_in *)&ifr.ifr_netmask;
	saddr->sin_family = AF_INET;
	saddr->sin_addr.s_addr = netmask;
	if (ioctl(sock_fd, SIOCSIFNETMASK, (void *)&ifr) < 0) {
		close(sock_fd);
		dbg("socket SIOCSIFNETMASK");
		exit(1);
	}
	dbg("set Netmask: ", tap_nic->dev.net_mask);
}

/**
 * @description: 把对应网络设备设置成持续模式，默认的虚拟网络设备，当其相关的文件符被关闭时，
 * 也将会伴随着与之相关的路由等信息同时消失。如果设置成持续模式，那么它将会被保留供以后使用。
 * @param {int} fd
 * @return {*}
 */
int TapIF::set_persist()
{
	/* if EBUSY, we donot set persist to tap */
	if (!errno && ioctl(tap_nic->fd, TUNSETPERSIST, 1) < 0) {
		dbg("ioctl TUNSETPERSIST");
		return -1;
	}
	return 0;
}

void TapIF::set_mod(){
	sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock_fd < 0){
		dbg("socket PF_INET");
		exit(1);
	}
}

void TapIF::unset_mod(){
	close(sock_fd);
	sock_fd = -1;
}

int TapIF::_tap_alloc(char *name)
{
	struct ifreq ifr;
	int tapfd;

	tapfd = open(TUNTAPDEV, O_RDWR);
	if (tapfd < 0) {
		dbg("can not open tapdev");
		return -1;
	}

	memset(&ifr, 0x0, sizeof(ifr));
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	if (*name)
		strncpy(ifr.ifr_name, name, IFNAMSIZ);
	/*
	 * create a new tap device
	 * if created already, just bind tun with file
	 */
	if (ioctl(tapfd, TUNSETIFF, (void *)&ifr) < 0) {
		dbg("ioctl TUNSETIFF");
		close(tapfd);
		return -1;
	}

	return tapfd;
}

void TapIF::_tap_free(){
    // delete tap_nic->dev.net_list;
    delete tap_nic;
}

