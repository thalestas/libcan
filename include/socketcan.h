#ifndef __SOCKETCAN_H__
#define __SOCKETCAN_H__

#include <iostream>
#include <net/if.h>
#include <linux/can.h>

class SocketCAN{
	private:
		//CAN connection variables
		int socket_fd{0};
		struct ifreq ifr;
		struct sockaddr_can addr;
		const char *ifname = "vcan0";

	public:
	
	//Implement constructors and destructors.
	SocketCAN();
	~SocketCAN();

	//Open SocketCAN
	int openSocket();

	//Check if SocketCAN is opened
	int isSocketOpened();

	//Close SocketCan
	void closeSocket();

	//Send pkg through the socket
	int writeSocket(struct can_frame *data);
};

#endif
