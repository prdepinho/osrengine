#pragma once

#include <vector>
#include <tuple>

class Tile;

enum class Size {
	None = -1,
	Tiny = 0,
	Small = 1,
	Medium = 2,
	Large = 3,
	Huge = 4,
	Gargantuan = 5
};

enum class Direction {
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4
};

struct Coordinates {
	int x;
	int y;
};

struct CharAttr {
	int abl_str;
	int abl_dex;
	int abl_con;
	int abl_int;
	int abl_wis;
	int abl_cha;
	int max_hp;
	int cur_hp;
	int speed;
	int prof_bonus;
	int initiative;
};

class Miniature {
public:
	Miniature(Size size=Size::Medium, CharAttr attr={}) : size(size), attr(attr) {}
	~Miniature() {}
	std::vector<Tile*> get_tiles() { return tiles; }
	void set_tiles(std::vector<Tile*> tiles) { this->tiles = tiles; }
	Size get_size() const { return size; }
	Coordinates get_coords() const { return Coordinates{x, y}; }
	void set_coords(int x, int y) { this->x = x; this->y = y; }
	int get_x() const { return x; }
	int get_y() const { return y; }
	CharAttr get_attr() const { return attr; }
private:
	Size size;
	std::vector<Tile*> tiles;
	int x;
	int y;
	CharAttr attr;
};


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
	Terrain(size_t width=0, size_t height=0);
	~Terrain(){}
	Tile *get_tile(int x, int y);

	std::vector<Miniature*> get_minies(int x, int y);
	std::vector<Miniature*> get_minies() { return miniatures; }
	bool put_mini(Miniature &mini, int x, int y);
	void take_mini(Miniature &mini);
	Coordinates find_mini(Miniature &mini);
	bool move_mini(Miniature &mini, Direction dir);
	bool move_mini(Miniature &mini, int x, int y);
	bool fits_mini(Miniature &mini, int x, int y);
private:
	size_t width;
	size_t height;
	std::vector<Tile> tiles;
	std::vector<Miniature*> miniatures;
};
