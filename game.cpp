#include "game.h"
#include <stdio.h>
#include <chrono>
#include <thread>

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
			game_mode->update();
			clear();
			game_mode->draw();
			refresh();
			if (game_mode->is_interactive()) {
				int ch = getch();
				game_mode->input(ch);
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}
}

void Game::init() {
	set_mode(new FreeMoveMode(this));
	initiate_dice();
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
	player = Miniature(Size::Medium, Character {
		10, 10, 10, 10, 10, 10,  // str, dev, con, int, wis, cha
		10, 10, 10, 30, 2, 0     // ac, max_hp, cur_hp, speed, prof_bonus, initiative
	});
	terrain.put_mini(player, 0, 0); 
	
	{
		Miniature npc(Size::Medium, Character {
				10, 10, 10, 10, 10, 10,
				10, 10, 10, 30, 2, 0
				});
		npcs.push_back(npc);
		terrain.put_mini(npcs.back(), 1, 2); 
	}

	cursor.x = player.get_x();
	cursor.y = player.get_y();

	astar_map = AStarMap(&terrain);
}

void Game::end() {
	endwin();
	if (game_mode) 
		delete game_mode;
	if (next_mode)
		delete next_mode;
}

bool Game::move_player(Direction dir) {
	bool rval = terrain.move_mini(player, dir);
	cursor.x = player.get_x();
	cursor.y = player.get_y();
	return rval;
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

std::stack<AStar::Direction> Game::search_path() {
	astar_map.set_mover(player);
	std::stack<AStar::Direction> path = AStar::search(
			astar_map,
		   	AStar::Vector2i(player.get_x(), player.get_y()),
		   	AStar::Vector2i(cursor.x, cursor.y));
	return path;
}
