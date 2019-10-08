#include <iostream>
#include "terrain.h"
#include "encounter.h"
#include <ncurses.h>
#include "log.h"
#include <thread>
#include <chrono>


struct Option {
	std::string label;
	char shortcut;
};

class Menu {
public:
	Menu() {}
	~Menu() {}
	int get_cursor() const { return cursor; }
	std::vector<Option> get_options() const { return options; }
	void move_cursor(int delta) {
	   	cursor += delta; 
		if (cursor < 0)
			cursor = 0;
		if (cursor >= options.size())
			cursor = options.size() -1;
	}
	void add_option(Option option) { options.push_back(option); }
	Option get_selected_option() const { return options[cursor]; }
	Option *get_option_by_shortcut(char shortcut) {
		for (Option &option : options) {
			if (option.shortcut == shortcut)
				return &option;
		}
		return nullptr;
	}
private:
	std::vector<Option> options;
	int cursor;
};

enum class InputMode {
	FreeMove,
	Action,
	Target,
	Wait
};

InputMode input_mode;
extern char _log_buffer[256];
bool loop = true;

int main(){

	Menu action_menu;
	action_menu.add_option(Option{"Move", 'm'});
	action_menu.add_option(Option{"Attack", 'a'});
	action_menu.add_option(Option{"Wait", 'w'});

	int cursor_x = 0;
	int cursor_y = 0;

	const size_t screen_width = 80;
	const size_t screen_height = 24;

	const size_t width = 20;
	const size_t height = 10;
	Terrain terrain(width, height);

	CharAttr attr;
	attr.abl_str = 10;
	attr.abl_dex = 10;
	attr.abl_con = 10;
	attr.abl_int = 10;
	attr.abl_wis = 10;
	attr.abl_cha = 10;
	attr.max_hp = 10;
	attr.cur_hp = 10;
	attr.speed = 30;
	attr.prof_bonus = 2;

	Miniature player(Size::Medium, attr);
	terrain.put_mini(player, 0, 6);

	Miniature dummy(Size::Medium);
	terrain.put_mini(dummy, 7, 3);

	Miniature bolder(Size::Large);
	terrain.put_mini(bolder, 2, 2);

	Encounter encounter(terrain);

	const int WHITE_BLACK = 1;
	const int BLACK_WHITE = 2;

	// init
	{
		initscr();
		raw();
		noecho();
		cbreak();
		start_color();
		keypad(stdscr, TRUE);
		curs_set(0);
		init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
		init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

		input_mode = InputMode::Action;
		cursor_x = player.get_x();
		cursor_y = player.get_y();
	}

	while (loop) {

		Miniature *mini = encounter.next_mini();
		if (mini == nullptr) {
			encounter.roll_initiative();
		}

		// draw
		{
			// clear screen
			for (int y = 0; y < screen_height; y++) {
				for (int x = 0; x < screen_width; x++) {
					mvaddch(y, x, ' ');
				}
			}

			// print log
			mvprintw(0, 0, _log_buffer);

			// print table
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int color = (x == cursor_x && y == cursor_y) ? BLACK_WHITE : WHITE_BLACK;
					Tile *tile = terrain.get_tile(x, y);

					char c = '.';
					if (tile->is_obstacle()) {
						c = '#';
					}
					for (Miniature *mini: terrain.get_minies(x, y)) {
						if (&player == mini) {
							c = '@';
						}
						else if (c != '@') {
							c = 'O';
						}
					}
					mvaddch(y + 1, x * 2 + 1, c | COLOR_PAIR(color));
				}
			}

			// print options
			{
				switch (input_mode) {
				case InputMode::Action:
					for (int y = 0; y < 10; y++) {
						mvaddch(y, screen_width -15, '|');
						if (y < action_menu.get_options().size()) {
							Option option = action_menu.get_options()[y];
							mvprintw(y, (screen_width -13), option.label.c_str());
						}
						if (y == action_menu.get_cursor()) {
							mvaddch(y, (screen_width -15), '>');
						}
					}
					break;
				case InputMode::Target:
					break;
				case InputMode::Wait:
					break;
				}
			}

			// print margin
			{
				for (int y = 0; y < screen_height; y++) {
					mvaddch(y, screen_width +1, '|');
				}
				for (int x = 0; x <= screen_width / 2; x++) {
					mvaddch(screen_height, x * 2, '-');
				}
			}

			// input
			{
				char ch;
				switch (input_mode) {
				case InputMode::Action:
					switch (ch = getch()) {
					case 'q':
						loop = false;
						break;
					case 'k':
						action_menu.move_cursor(-1);
						break;
					case 'j':
						action_menu.move_cursor(1);
						break;
					case '\r':
					case '\n':
						{
							Option option = action_menu.get_selected_option();
							log("Option chosen: %s", option.label.c_str());
						}
						input_mode = InputMode::FreeMove;
						break;
					default:
						{
							Option *option = action_menu.get_option_by_shortcut(ch);
							if (option) {
								log("Option chosen: %s", option->label.c_str());
							}
						}
						break;
					}
					break;

				case InputMode::FreeMove:
					switch (ch = getch()) {
					case 'q':
						loop = false;
						break;
					case 'h':
						if(terrain.move_mini(player, Direction::Left))
							cursor_x -= 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'j':
						if(terrain.move_mini(player, Direction::Down))
							cursor_y += 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'k':
						if (terrain.move_mini(player, Direction::Up))
							cursor_y -= 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'l':
						if (terrain.move_mini(player, Direction::Right))
							cursor_x += 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 't':
						input_mode = InputMode::Target;
						break;
					case 'b':
						input_mode = InputMode::Action;
						cursor_x = player.get_x();
						cursor_y = player.get_y();
						break;
					default:
						log("%d", ch);
						break;
					}
					break;

				case InputMode::Target:
					switch (ch = getch()) {
					case 'q':
						loop = false;
						break;
					case 'h':
						cursor_x -= 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'j':
						cursor_y += 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'k':
						cursor_y -= 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case 'l':
						cursor_x += 1;
						log("cursor: %d, %d", cursor_x, cursor_y);
						break;
					case '\r':
					case '\n':
						input_mode = InputMode::Action;
						cursor_x = player.get_x();
						cursor_y = player.get_y();
						break;
					case 'b':
						input_mode = InputMode::Action;
						cursor_x = player.get_x();
						cursor_y = player.get_y();
						break;
					}
					break;

				case InputMode::Wait:
					std::this_thread::sleep_for(std::chrono::seconds(1));
					break;
				}
			}

			refresh();
		}

	}
	endwin();

	return 0;
}
