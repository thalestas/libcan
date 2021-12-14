#include "utils.h"
#include <iostream>
#include <random>

unsigned randomNumber(unsigned down, unsigned up) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> distr(down, up);

	return distr(gen);
}

unsigned speedSimulator() {
	static enum {UP, DOWN} mode{UP};
	static unsigned speed{0};

	switch (mode){
		case UP:
			if(speed <= 12000)
				speed += randomNumber(0x01, 0xFF);
			else
				mode = DOWN;
			break;
		case DOWN:
			if(speed >= 6000)
				speed -= randomNumber(0x01, 0xFF);
			else
				mode = UP;
			break;
		}

	return speed;
}

unsigned tempSimulator(bool air) {
	static unsigned temp{0};

	switch (air) {
		case true:
			if(temp >= 160)
				temp -= randomNumber(0x01, 0x0A);
			else
				temp += randomNumber(0x01,0x0A);
			break;
		case false:
			if(temp <= 300)
				temp += randomNumber(0x01,0x0A);
			break;
	}
	
	return temp;
}
