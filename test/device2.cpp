#include <iostream>
#include <iomanip>
#include <linux/can.h>
#include "socketcan.h"

const unsigned SPEED_ADDR{0x123};
const unsigned TEMP_ADDR{0x12F};
const unsigned AIR_ADDR{0x1BC};
const unsigned PKG_TIMEOUT{5};

void controlAir(bool air, SocketCAN *skt) {
	struct can_frame frame;
	frame.can_id = AIR_ADDR;
	frame.can_dlc = 1;
	frame.data[0] = air;

	skt->writeSocket(&frame);
}

void printStats(double speed, double temp) {
	std::cout << "Speed: " 
		  << std::fixed 
		  << std::setprecision(1) 
		  << std::setw(4)
		  << speed/100 << " km/h"
		  << "    ";
	std::cout << "Temp: " 
		  << std::fixed 
		  << std::setprecision(1) 
		  << std::setw(4)
		  << temp/10 << " deg C";
	std::cout << std::endl;
}

void pkgHandler(const struct can_frame &frame, SocketCAN *skt) {
	static double speed{0};
	static double temp{0};

	switch (frame.can_id){
		case SPEED_ADDR:
			speed = frame.data[1];
			speed += frame.data[0] << 8;
			break;
		case TEMP_ADDR:
			temp = frame.data[1];
			temp += frame.data[0] << 8;

			if(temp > 250)
				controlAir(true, skt);
			else
				controlAir(false, skt);
			break;
	}
	printStats(speed, temp);
}



int main() {
	//Open SocketCAN
	SocketCAN *vcan{new SocketCAN};
	
	//Create Filter
	struct can_filter *cfilter {new struct can_filter[2]};
	cfilter[0].can_id = SPEED_ADDR;
	cfilter[0].can_mask = (CAN_SFF_MASK);
	cfilter[1].can_id = TEMP_ADDR;
	cfilter[1].can_mask = (CAN_SFF_MASK);
	vcan->setFilter(cfilter);

	//SocketCAN frame
	struct can_frame frame;

	while(1) {
		if(vcan->selectSocket(PKG_TIMEOUT)) {
			if(vcan->readSocket(&frame))
				pkgHandler(frame, vcan);
		}
	}

	delete [] cfilter;

	return 0;
}
