#include <iostream>
#include "terrain.h"
#include <ncurses.h>

int main(){
	size_t width = 8;
	size_t height = 6;
	Terrain terrain(width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile &tile = terrain.get_tile(x, y);
			std::cout << " " << tile.get_x() << "," << tile.get_y();
		}
		std::cout << std::endl;
	}

	std::cout << "Hello, world!" << std::endl;
	return 0;
}
