#include "dice.h"

void Dice::initiate() {
	std::srand(std::time(0));
}

int Dice::roll(int sides) {
	return (std::rand() % sides) + 1;
}
