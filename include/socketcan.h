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
	//Constructors and destructors.
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
	
	//Receive pkg through the socket
	int readSocket(struct can_frame *data);

	//Check if a new pkg was received
	int selectSocket(int sec_timeout);

	//Print the frame
	void printFrame(const struct can_frame &frame);
};

#endif
