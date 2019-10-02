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
