#pragma once

#include <ncurses.h>
#include "terrain.h"

class Game;


class GameMode {
public:
	GameMode(Game *game=nullptr) : game(game) {}
	~GameMode(){}
	virtual void input(int ch);
	virtual void draw();
protected:
	Game *game;
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

class MovementMode : public SelectTileMode {
public:
   	MovementMode(Game *game=nullptr) : SelectTileMode(game) {}
	virtual void input(int ch);
};
