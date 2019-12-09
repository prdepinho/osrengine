#include <iostream>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "terrain.h"
#include "encounter.h"
#include "log.h"
#include "astar.h"
#include "dice.h"

enum class GameState {
	MenuInteract,
	BoardInteract,
	Effect
};

enum class OptionType {
	ACTION_MOVE,
	ACTION_ATTACK,
	ACTION_WAIT,
	ACTION_PASS
};

struct Option {
	OptionType type;
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

Menu action_menu;
Coordinates cursor;
Coordinates chosen;
std::vector<Coordinates> highlight;
Miniature *current_mini = nullptr;
const size_t screen_width = 80;
const size_t screen_height = 24;
const size_t width = 20;
const size_t height = 10;
Terrain terrain(width, height);
CharAttr attr;
Miniature player(Size::Medium, attr);
const int WHITE_BLACK = 1;
const int BLACK_WHITE = 2;
InputMode input_mode;
extern char _log_buffer[256];
bool loop = true;
Encounter encounter(terrain);
GameState state = GameState::MenuInteract;

class LoopState {
public:
	virtual void update() {}
	virtual void input(char ch) {}
private:
};

class SelectOptionState : public LoopState {
public:
	virtual void input(char ch) override {
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
					Log("Option chosen: %s", option.label.c_str());
				}
				input_mode = InputMode::FreeMove;
				break;
			default:
				{
					Option *option = action_menu.get_option_by_shortcut(ch);
					if (option) {
						Log("Option chosen: %s", option->label.c_str());
						switch (option->type) {
							case OptionType::ACTION_MOVE:
								input_mode = InputMode::Target;
								break;
							case OptionType::ACTION_ATTACK:
								input_mode = InputMode::Target;
								break;
							case OptionType::ACTION_WAIT:
								break;
							case OptionType::ACTION_PASS:
								break;
						}
					}
				}
				break;
		}
	}
private:
};

class SelectTileState : public LoopState {
public:
private:
};

class FreeMoveState : public LoopState {
public:
private:
};


int main(){

	Dice::initiate();

	action_menu.add_option(Option{OptionType::ACTION_MOVE, "Move", 'm'});
	action_menu.add_option(Option{OptionType::ACTION_ATTACK, "Attack", 'a'});
	action_menu.add_option(Option{OptionType::ACTION_WAIT, "Wait", 'w'});
	action_menu.add_option(Option{OptionType::ACTION_PASS, "Pass", 'p'});

	cursor.x = 0;
	cursor.y = 0;

	chosen.x = 0;
	chosen.y = 0;


	// const Action no_action{0};
	// Action prepared_action = no_action;

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

	terrain.put_mini(player, 0, 6);

	// Miniature dummy(Size::Medium);
	// terrain.put_mini(dummy, 7, 3);

	// Miniature bolder(Size::Large);
	// terrain.put_mini(bolder, 2, 2);


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
		cursor.x = player.get_x();
		cursor.y = player.get_y();

		current_mini = &player;
	}

	while (loop) {
		switch (state) {
		case GameState::Effect:
			break;
		case GameState::MenuInteract:
			break;
		case GameState::BoardInteract:
			break;
		}
	}

#if false
	while (loop) {

		Miniature *mini = encounter.next_mini();
		if (mini == nullptr) {
			encounter.roll_initiative();
		}

		// draw
		{
			// clear screen
			clear();

			// print log
			mvprintw(0, 0, _log_buffer);

			// print table
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					int color = (x == cursor.x && y == cursor.y) ? BLACK_WHITE : WHITE_BLACK;
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
					break;

				case InputMode::FreeMove:
					switch (ch = getch()) {
					case 'q':
						loop = false;
						break;
					case 'h':
						if(terrain.move_mini(player, Direction::Left))
							cursor.x -= 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'j':
						if(terrain.move_mini(player, Direction::Down))
							cursor.y += 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'k':
						if (terrain.move_mini(player, Direction::Up))
							cursor.y -= 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'l':
						if (terrain.move_mini(player, Direction::Right))
							cursor.x += 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 't':
						input_mode = InputMode::Target;
						break;
					case 'b':
						input_mode = InputMode::Action;
						cursor.x = player.get_x();
						cursor.y = player.get_y();
						break;
					default:
						Log("%d", ch);
						break;
					}
					break;

				case InputMode::Target:
					switch (ch = getch()) {
					case 'q':
						loop = false;
						break;
					case 'h':
						cursor.x -= 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'j':
						cursor.y += 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'k':
						cursor.y -= 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case 'l':
						cursor.x += 1;
						Log("cursor: %d, %d", cursor.x, cursor.y);
						break;
					case '\r':
					case '\n':
						input_mode = InputMode::Action;
						cursor.x = player.get_x();
						cursor.y = player.get_y();
						break;
					case 'b':
						input_mode = InputMode::Action;
						cursor.x = player.get_x();
						cursor.y = player.get_y();
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
#endif
	endwin();

	return 0;
}
