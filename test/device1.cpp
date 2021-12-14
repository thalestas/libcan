#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include "utils.h"
#include <memory>
#include <mutex>
#include "socketcan.h"

const unsigned SPEED_ADDR{0x123};
const unsigned TEMP_ADDR{0x12F};
const unsigned AIR_ADDR{0x1BC};
const unsigned PKG_TIMEOUT{5};
const unsigned PKG_SIZE{2};

std::mutex mtx;

void sendPkg(SocketCAN *skt, std::shared_ptr<bool> air) {
	std::shared_ptr<bool> air_status = air;
	struct can_frame frame[2];

	unsigned speed{0};
	unsigned temp{0};

	frame[0].can_id = SPEED_ADDR;
	frame[0].can_dlc = PKG_SIZE;

	frame[1].can_id = TEMP_ADDR;
	frame[1].can_dlc = PKG_SIZE;

	while(1) {
		mtx.lock();
		for(auto f : frame)
			skt->writeSocket(&f);
		mtx.unlock();

		speed = speedSimulator();
		frame[0].data[0] = speed >> 8;
		frame[0].data[1] = speed;

		temp = tempSimulator(*air_status);
		frame[1].data[0] = temp >> 8;
		frame[1].data[1] = temp;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void pkgHandler(SocketCAN *skt, std::shared_ptr<bool> air) {
	std::shared_ptr<bool> air_status = air;
	
	//SocketCAN frame
	struct can_frame frame;

	while(1){
		std::cout << "Air status: " << *air << std::endl;
		if(skt->selectSocket(PKG_TIMEOUT)) {
			if(skt->readSocket(&frame)) {
				if(frame.can_id == AIR_ADDR){
					*air = frame.data[0];	
				}
			}
		}
	}
}

int main() {
	//Open SocketCAN
	SocketCAN *vcan{new SocketCAN};
	
	//Create Filter
	struct can_filter *cfilter {new struct can_filter};
	cfilter[0].can_id = AIR_ADDR;
	cfilter[0].can_mask = (CAN_SFF_MASK);
	vcan->setFilter(cfilter);

	//Shared pointer to control the air
	auto air = std::make_shared<bool>(bool{false});

	//Thread to send pkgs
	std::thread t1(sendPkg, vcan, air);
	std::thread t2(pkgHandler, vcan, air);

	t1.join();
	t2.join();

	return 0;
}
