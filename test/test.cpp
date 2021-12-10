#include <iostream>
#include "socketcan.h"

int main() {
	SocketCAN tst;
	tst.openSocket();
	tst.closeSocket();

	return 0;
}
