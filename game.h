#pragma once

#include <ncurses.h>
#include "dice.h"
#include "terrain.h"
#include "encounter.h"
#include "gamemode.h"
#include "log.h"

const int WHITE_BLACK = 1;
const int BLACK_WHITE = 2;

const size_t screen_width = 80;
const size_t screen_height = 24;
const size_t width = 20;
const size_t height = 10;

class Game {
public:
	Game() {}
	~Game() { if (running) end(); }
	void start();
	void stop() { running = false; }
	void set_mode(GameMode *mode);
	void move_player(Direction dir);
	void move_cursor(Direction dir);
	void reset_cursor();
	void select_tile();
	Terrain terrain;
	Miniature player;
	Coordinates cursor;
private:
	void init();
	void loop();
	void end();
	bool running = true;
	GameMode *game_mode = nullptr;
	GameMode *next_mode = nullptr;
	void switch_mode();
	Coordinates chosen;
};

