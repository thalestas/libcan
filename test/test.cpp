#include <iostream>
#include "socketcan.h"
#include <linux/can.h>

int main() {
	SocketCAN tst;

	struct can_frame frame;

	frame.can_id = 0x123;
	frame.can_dlc = 2;
	frame.data[0] = 0xBB;
	frame.data[1] = 0x1F;

	tst.writeSocket(&frame);

	return 0;
}
