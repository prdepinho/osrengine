#include "terrain.h"

Terrain::Terrain(size_t width=0, size_t height=0)
	: width(width), height(height) 
{
	tiles = std::vector<Tile>(width * height);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			tiles[x + y * width] = {x, y, false};
		}
	}
}

Tile *Terrain::get_tile(int x, int y) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return nullptr;
	}
   	return &tiles[x + y * width];
}

std::vector<Miniature*> Terrain::get_minies(int x, int y) {
	std::vector<Miniature*> minies;
	for (Miniature *mini: miniatures) {
		Coordinates coords = mini->get_coords();
		if (coords.x == x && coords.y == y) {
			minies.push_back(mini);
		}
	}
	return minies;
}

void Terrain::put_mini(Miniature &mini, int x, int y) {
	switch (mini.get_size()) {
		case Size::Tiny:
		case Size::Small:
		case Size::Medium:
			mini.set_coords(x, y);
			miniatures.push_back(&mini);
			break;
		case Size::Large:
			break;
		case Size::Gargantuan:
			break;
	}
}

void Terrain::take_mini(Miniature &mini) {
	for (auto it = miniatures.begin(); it != miniatures.end(); ++it) {
		if (*it == &mini) {
			it = miniatures.erase(it);
		}
	}
}

Coordinates Terrain::find_mini(Miniature &mini) {
	return mini.get_coords();
}

void Terrain::move_mini(Miniature &mini, Direction dir) {
	Tile *tile;
	Coordinates coords = mini.get_coords();
	switch (dir) {
		case Direction::Up:
			tile = get_tile(coords.x, coords.y - 1);
			break;
		case Direction::Down:
			tile = get_tile(coords.x, coords.y + 1);
			break;
		case Direction::Left:
			tile = get_tile(coords.x - 1, coords.y);
			break;
		case Direction::Right:
			tile = get_tile(coords.x + 1, coords.y);
			break;
	}
	if (tile != nullptr && !tile->is_obstacle()) {
		mini.set_coords(tile->get_x(), tile->get_y());
	}
}
