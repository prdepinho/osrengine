#include "dice.h"

int Dice::roll() {
	int total = 0;
	for (int i = 0; i < quantity; i++) {
		total += roll_die(sides);
	}
	return total;
}

void initiate_dice() {
	std::srand(std::time(0));
}

int roll_die(int sides) {
	return (std::rand() % sides) + 1;
}
