#include "socketcan.h"

#include <net/if.h>
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>

SocketCAN::SocketCAN() {
	SocketCAN::openSocket();
}

SocketCAN::~SocketCAN() {
	SocketCAN::closeSocket();
}

int SocketCAN::openSocket() {
	//Create SocketCAN
	if ((socket_fd=socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
		std::cout << "Could not create SocketCAN" << std::endl;
		return socket_fd;
	}
	std::cout << "Created CAN socket with id: " << socket_fd << std::endl;

	//Get network interface index
	std::strcpy(ifr.ifr_name, ifname);
	if(ioctl(socket_fd, SIOCGIFINDEX, &ifr) == -1) {
		std::cout << "Unable to select CAN interface: " << ifr.ifr_name << std::endl;
		closeSocket();
		return -1;
	}
	std::cout << "Selected SocketCAN: " << ifr.ifr_name << std::endl;

	//Bind the SocketCAN
	addr.can_family = AF_CAN;	
	addr.can_ifindex = ifr.ifr_ifindex;
	if(bind(socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
		std::cout << "Unable to bind CAN interface: " << ifr.ifr_name << std::endl;
		closeSocket();
		return -1;
	}

	std::cout << "SocketCAN opened!" << std::endl;
}

int SocketCAN::isSocketOpened() {
	return socket_fd;
}

void SocketCAN::closeSocket() {
	if(isSocketOpened() == -1)
		return;
	close(socket_fd);
	socket_fd = -1;
	std::cout << "SocketCAN closed!" << std::endl;
}

int SocketCAN::writeSocket(struct can_frame *data) {
	int nbytes = write(socket_fd, data, sizeof(struct can_frame));
	return nbytes;
}
