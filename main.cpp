#include <iostream>
#include <thread>
#include <chrono>
#include "terrain.h"
#include "encounter.h"
#include "log.h"
#include "astar.h"
#include "dice.h"
#include "gamemode.h"
#include "game.h"

Terrain terrain(width, height);
Miniature player(Size::Medium, CharAttr());


int main() {

	Game game;
	game.start();

	return 0;
}

