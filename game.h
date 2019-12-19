#pragma once

#include <ncurses.h>
#include "dice.h"
#include "terrain.h"
#include "encounter.h"
#include "gamemode.h"
#include "log.h"
#include "action.h"
#include "astar.h"
#include "rules.h"

const int WHITE_BLACK = 1;
const int BLACK_WHITE = 2;

const size_t screen_width = 80;
const size_t screen_height = 24;
const size_t width = 20;
const size_t height = 10;

class AStarMap : public AStar::Matrix {
public:
	AStarMap(Terrain *terrain=nullptr) : terrain(terrain) {}
	void set_mover(Miniature &miniature) { mover = &miniature; }
	virtual float get(int x, int y) { return (terrain->fits_mini(*mover, x, y) ? .0f : 1.f); }
	virtual size_t get_width() const { return terrain->get_width(); }
	virtual size_t get_height() const { return terrain->get_height(); }
private:
	Terrain *terrain;
	Miniature *mover;
};


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
	void set_action(Action *action);
	void act();
	std::stack<AStar::Direction> search_path();
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
	Action *chosen_action = nullptr;
	AStarMap astar_map;
};

