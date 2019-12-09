#pragma once

#include "terrain.h"
#include <vector>

struct Initiative {
	Miniature *mini;
	int initiative;
};

class Encounter {
public:
	Encounter(Terrain &terrain) : terrain(&terrain) {}
	~Encounter() {}
	void roll_initiative();
	Miniature *next_mini();
private:
	std::vector<Initiative> initiative_order;
	Terrain *terrain;
};
