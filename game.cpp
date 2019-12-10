#include "game.h"
#include <stdio.h>

void Game::set_mode(GameMode *mode) {
	next_mode = mode;
}

void Game::start() {
	init();
	loop();
	end();
}

void Game::switch_mode() {
	if (next_mode) {
		if (game_mode)
			delete game_mode;
		game_mode = next_mode;
		next_mode = nullptr;
	}
}

void Game::loop() {
	while (running) {
		switch_mode();
		if (game_mode) {
			game_mode->draw();
			int ch = getch();
			game_mode->input(ch);
		}
	}
}

void Game::init() {
	set_mode(new GameMode(this));
	Dice::initiate();
	initscr();
	raw();
	noecho();
	cbreak();
	start_color();
	keypad(stdscr, TRUE);
	curs_set(0);
	init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

	chosen.x = 0;
	chosen.y = 0;

	terrain = Terrain(width, height);
	player = Miniature(Size::Medium, CharAttr());
	terrain.put_mini(player, 0, 0); 
	cursor.x = player.get_x();
	cursor.y = player.get_y();
}

void Game::end() {
	endwin();
	if (game_mode) 
		delete game_mode;
	if (next_mode)
		delete next_mode;
}

void Game::move_player(Direction dir) {
	terrain.move_mini(player, dir);
	cursor.x = player.get_x();
	cursor.y = player.get_y();
}

void Game::move_cursor(Direction dir) {
	Tile *tile;
	switch (dir) {
		case Direction::Up:
			tile = terrain.get_tile(cursor.x, cursor.y - 1);
			break;
		case Direction::Down:
			tile = terrain.get_tile(cursor.x, cursor.y + 1);
			break;
		case Direction::Left:
			tile = terrain.get_tile(cursor.x - 1, cursor.y);
			break;
		case Direction::Right:
			tile = terrain.get_tile(cursor.x + 1, cursor.y);
			break;
	}
	if (tile) {
		cursor.x = tile->get_x();
		cursor.y = tile->get_y();
	}
}

void Game::reset_cursor() {
	cursor.x = player.get_x();
	cursor.y = player.get_y();
}

void Game::set_action(Action *action) {
	if (chosen_action)
		delete chosen_action;
	chosen_action = action;
}

void Game::act() {
	if (chosen_action)
		chosen_action->effect(terrain, player, cursor);
}