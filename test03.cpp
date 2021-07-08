/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 17:49:54
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-06-30 00:08:03
 */
#include <iostream>
#include <stdint.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/unistd.h>

int main(int argc, char const *argv[])
{
	struct ifreq ifr;
	int tapfd;

    char* name = "tap0";

	tapfd = open("/dev/net/tun", O_RDWR);
	if (tapfd < 0) {
		// dbg("can not open tapdev");
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
		// dbg("ioctl TUNSETIFF");
		close(tapfd);
		return -1;
	}
}
