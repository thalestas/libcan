#include "socketcan.h"

#include <net/if.h>
#include <sys/socket.h>
#include <linux/can/raw.h>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/select.h>

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
	return socket_fd;
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

int SocketCAN::readSocket(struct can_frame *data) {
	int nbytes = read(socket_fd, data, sizeof(struct can_frame));
	
	if(nbytes < 0)
		std::cout << "Pkg Error!" << std::endl;
	else if(nbytes < sizeof(struct can_frame)) {
		std::cout << "Incomplete pkg!" << std::endl;
		nbytes = 0;
	}
	return nbytes;
}

int SocketCAN::selectSocket(int sec_timeout) {
	struct timeval tv;
	int retval{0};
	fd_set rfds;

	tv.tv_sec = sec_timeout;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_SET(socket_fd, &rfds);

	retval = select(socket_fd+1, &rfds, NULL, NULL, &tv);

	if(retval == -1)
		std::cout << "error select" << std::endl;
	else if(!retval)
		std::cout << "Timeout!!" << std::endl;

	return retval;
}

void SocketCAN::printFrame(const struct can_frame &frame) {
	std::cout << std::hex << "id: " << frame.can_id;
	std::cout << " len: " << static_cast<unsigned>(frame.can_dlc);
	std::cout << " data: ";
	for(int i=0; i<static_cast<unsigned>(frame.can_dlc); i++)
		std::cout << std::uppercase << static_cast<unsigned>(frame.data[i]) << " ";
	std::cout << std::endl;
}

int SocketCAN::setFilter(const struct can_filter *filter) {
	if(!SocketCAN::isSocketOpened())
		return -1;

	return setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, filter, sizeof(filter)/sizeof(struct can_filter));

}
