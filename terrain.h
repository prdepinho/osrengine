#pragma once

#include <vector>

class Tile {
public:
	Tile(int x=0, int y=0, bool obstacle=false) : x(x), y(y), obstacle(obstacle) {}
	~Tile() {}
	int get_x() const { return x; }
	int get_y() const { return y; }
	bool is_obstacle() const { return obstacle; }
private:
	int x;
	int y;
	bool obstacle;
};

class Terrain {
public:
	Terrain(size_t width, size_t height);
	~Terrain(){}
	Tile &get_tile(int x, int y) { return tiles[x + y * width]; }
private:
	size_t width;
	size_t height;
	std::vector<Tile> tiles;
};
