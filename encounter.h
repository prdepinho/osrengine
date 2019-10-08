#pragma once

#include "terrain.h"

class Encounter {
public:
	Encounter(Terrain &terrain) : terrain(&terrain) {}
	~Encounter() {}
	void roll_initiative();
	Miniature *next_mini();
private:
	Terrain *terrain;
};
