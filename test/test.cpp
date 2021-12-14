#include <ios>
#include <iostream>
#include "socketcan.h"
#include <linux/can.h>

int main() {
	SocketCAN tst;
	struct can_frame frame;

	//pkg to send
	frame.can_id = 0x123;
	frame.can_dlc = 2;
	frame.data[0] = 0xBB;
	frame.data[1] = 0x1F;

	//send pkg
	tst.writeSocket(&frame);

	struct can_filter filter;
	filter.can_id = 0x009;
	filter.can_mask = (CAN_EFF_FLAG | CAN_RTR_FLAG | CAN_SFF_MASK);
	tst.setFilter(&filter);

	//Loop to read pkg
	std::cout << "Read pkg.." << std::endl;
	int nbytes{0};
	int select_retval{0};
	while(1){
		if(tst.selectSocket(5)) {
			nbytes = tst.readSocket(&frame);
			if(nbytes) 
				tst.printFrame(frame);
		}
	}

	return 0;
}
