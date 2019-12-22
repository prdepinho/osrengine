#pragma once
#include "terrain.h"
#include <vector>
#include "terrain.h"
#include "log.h"
#include <sstream>

typedef std::vector<Tile*> AOE;

class Action {
public:
	virtual ~Action() {}
	virtual void effect(Terrain &terrain, Miniature &actor, Coordinates target) = 0;
};


class EchoAction : public Action {
public:
	virtual void effect(Terrain &terrain, Miniature &actor, Coordinates target) {
		std::stringstream ss;
		ss << "Actor: " << actor.get_x() << ", " << actor.get_y() << "; ";
		ss << "Target: " << target.x << ", " << target.y << ".";
		Log::write(ss.str());
	}
};
