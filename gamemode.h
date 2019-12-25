#pragma once

#include <ncurses.h>
#include "terrain.h"
#include "astar.h"

class Game;


class GameMode {
public:
	GameMode(Game *game=nullptr, bool interactive=true) : game(game), interactive(interactive) {}
	virtual ~GameMode() {}
	virtual void input(int ch);
	virtual void draw();
	virtual void update() {}
	bool is_interactive() const { return interactive; }
protected:
	Game *game;
	bool interactive;
};


class FreeMoveMode : public GameMode {
public:
	FreeMoveMode(Game *game=nullptr);
	virtual void input(int ch);
};

class SelectTileMode : public GameMode {
public:
	SelectTileMode(Game *game=nullptr) : GameMode(game) {}
	virtual void input(int ch);
};


class SelectActionTargetMode : public SelectTileMode {
public:
	SelectActionTargetMode(Game *game=nullptr) : SelectTileMode(game) {}
	virtual void input(int ch);
};


class MovementSelectMode : public SelectTileMode {
public:
	MovementSelectMode(Game *game=nullptr) : SelectTileMode(game) {}
	virtual void input(int ch);
};


class MovementEffectMode : public GameMode {
public:
	MovementEffectMode(AStar::Path path, Game *game=nullptr) : GameMode(game, false), path(path) {}
	virtual void update();
protected:
	AStar::Path path;
};



class AttackSelectMode : public SelectTileMode {
public:
	AttackSelectMode(Game *game=nullptr) : SelectTileMode(game) {}
	virtual void input(int ch);
};
