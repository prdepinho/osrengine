#include <iostream>
#include "terrain.h"
#include <ncurses.h>

int main(){
	size_t width = 20;
	size_t height = 10;
	Terrain terrain(width, height);

	Miniature player(Size::Large);
	terrain.put_mini(player, 0, 6);


	Miniature bee(Size::Tiny);
	terrain.put_mini(bee, 9, 3);

	Miniature chair(Size::Small);
	terrain.put_mini(chair, 5, 3);

	Miniature dummy(Size::Medium);
	terrain.put_mini(dummy, 7, 3);

	Miniature bolder(Size::Large);
	terrain.put_mini(bolder, 2, 2);

	Miniature house(Size::Huge);
	terrain.put_mini(house, 5, 5);

	Miniature collosus(Size::Gargantuan);
	terrain.put_mini(collosus, 9, 5);

	initscr();
	raw();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	const int WHITE_BLACK = 1;
	init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);

	bool loop = true;
	while (loop) {

		// draw
		{
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
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
					mvaddch(y + 2, x * 2 + 2, c | COLOR_PAIR(WHITE_BLACK));
				}
			}
			refresh();
		}

		// input
		{
			int ch = getch();
			switch (ch) {
				case 'q':
					loop = false;
					break;
				case 'h':
					terrain.move_mini(player, Direction::Left);
					break;
				case 'j':
					terrain.move_mini(player, Direction::Down);
					break;
				case 'k':
					terrain.move_mini(player, Direction::Up);
					break;
				case 'l':
					terrain.move_mini(player, Direction::Right);
					break;
			}
		}
	}
	endwin();

	return 0;
}
