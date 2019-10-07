#include <math.h>
#include "terrain.h"
#include "log.h"

Terrain::Terrain(size_t width, size_t height)
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
		bool contains = false;
		Coordinates coords = mini->get_coords();
		switch (mini->get_size()) {
		case Size::Tiny:
		case Size::Small:
		case Size::Medium:
			contains = (coords.x == x && coords.y == y);
			break;
		case Size::Large:
			contains = ((coords.x == x || coords.x == x - 1) && (coords.y == y || coords.y == y - 1));
			break;
		case Size::Huge:
			contains = (coords.x <= x && coords.x >= x - 2 && coords.y <= y && coords.y >= y - 2);
			break;
		case Size::Gargantuan:
			contains = (coords.x <= x && coords.x >= x - 3 && coords.y <= y && coords.y >= y - 3);
			break;
		}
		if (contains) {
			minies.push_back(mini);
		}
	}
	return minies;
}

bool Terrain::put_mini(Miniature &mini, int x, int y) {
	if (fits_mini(mini, x, y)) {
		mini.set_coords(x, y);
		miniatures.push_back(&mini);
		return true;
	}
	return false;
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

bool Terrain::move_mini(Miniature &mini, Direction dir) {
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
	if (tile != nullptr && fits_mini(mini, tile->get_x(), tile->get_y())) {
		mini.set_coords(tile->get_x(), tile->get_y());
		return true;
	}
	return false;
}

bool Terrain::fits_mini(Miniature &mini, int target_x, int target_y) {
	int tiles_size;
	switch (mini.get_size()) {
	case Size::Tiny:
	case Size::Small:
	case Size::Medium:
		tiles_size = 1;
		break;
	case Size::Large:
		tiles_size = 2;
		break;
	case Size::Huge:
		tiles_size = 3;
		break;
	case Size::Gargantuan:
		tiles_size = 4;
		break;
	}

	bool fits = true;
	for (int x = target_x; x < target_x + tiles_size; x++) {
		for (int y = target_y; y < target_y + tiles_size; y++) {
			Tile *tile = get_tile(x, y);
			fits = fits && (tile != nullptr && !tile->is_obstacle());
			for (Miniature *put_mini: get_minies(x, y)) {
				if (put_mini != &mini) {
					fits = fits && std::abs(((int)put_mini->get_size() - (int)mini.get_size())) >= 2;
				}
			}
		}
	}
	return fits;
}
