#pragma once
#include <cstdlib>
#include <ctime>

class Dice {
public:
	Dice(int quantity=0, int sides=0) : quantity(quantity), sides(sides) {}
	int roll();
private:
	int quantity;
	int sides;
};

void initiate_dice();

int roll_die(int sides);
