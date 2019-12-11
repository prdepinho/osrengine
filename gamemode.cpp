#include "gamemode.h"
#include "game.h"
#include "action.h"

void GameMode::input(int ch) {
	switch (ch) {
	case 'q':
		game->stop();
		break;
	case 't':
		Log::write("Target");
		game->set_action(new EchoAction());
		game->set_mode(new SelectActionTargetMode(game));
		break;
	case 'm':
		Log::write("Move");
		game->set_mode(new MovementMode(game));
		break;
	case '0':
		Log::write("Game Mode");
		game->set_mode(new GameMode(game));
		break;
	case '1':
		Log::write("Free Move Mode");
		game->set_mode(new FreeMoveMode(game));
		break;
	case '2':
		Log::write("Select Tile Mode");
		game->set_mode(new SelectTileMode(game));
		break;
	}
}

void GameMode::draw() {
	clear();
	// draw map
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int color = (x == game->cursor.x && y == game->cursor.y) ? BLACK_WHITE : WHITE_BLACK;
			Tile *tile = game->terrain.get_tile(x, y);

			char c = '.';
			if (tile->is_obstacle()) {
				c = '#';
			}
			for (Miniature *mini: game->terrain.get_minies(x, y)) {
				if (&game->player == mini) {
					c = '@';
				}
				else if (c != '@') {
					c = 'O';
				}
			}
			mvaddch(y + 1, x * 2 + 1, c | COLOR_PAIR(color));
		}
	}

	// draw log
	{
		int y = screen_height - 2;
		int x = 1;
		for (std::string &line : Log::get_lines(3)) {
			mvprintw(y++, x, line.c_str());
		}
	}
	refresh();
}


FreeMoveMode::FreeMoveMode(Game *game) : GameMode(game) {
	game->reset_cursor();
}

void FreeMoveMode::input(int ch) {
	GameMode::input(ch);
	switch (ch) {
		case 'k':
			game->move_player(Direction::Up);
			break;
		case 'j':
			game->move_player(Direction::Down);
			break;
		case 'h':
			game->move_player(Direction::Left);
			break;
		case 'l':
			game->move_player(Direction::Right);
			break;
	}
}


void SelectTileMode::input(int ch) {
	GameMode::input(ch);
	switch (ch) {
		case 'k':
			game->move_cursor(Direction::Up);
			break;
		case 'j':
			game->move_cursor(Direction::Down);
			break;
		case 'h':
			game->move_cursor(Direction::Left);
			break;
		case 'l':
			game->move_cursor(Direction::Right);
			break;
		case '\r':
		case '\n':
			Log::write(std::to_string(game->cursor.x) + ", " + std::to_string(game->cursor.y));
			break;
	}
}


void MovementMode::input(int ch) {
	SelectTileMode::input(ch);
	switch(ch) {
		case '\r':
		case '\n':
			Log::write("Action");
			game->act();
			game->set_mode(new FreeMoveMode(game));
			break;
	}
}

void MovementMode::input(int ch) {
	switch(ch) {
		case '\r':
		case '\n':
			Log::write("Move to: " + std::to_string(game->cursor.x) + ", " + std::to_string(game->cursor.y));
			break;
	}
	SelectTileMode::input(ch);
}
