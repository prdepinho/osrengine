#pragma once
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <stack>
#include <map>
#include <complex>

namespace AStar {

	// These directives define if the algorithm stops when it finds the destiny, not giving the optimal path, but executing much faster.
	// In order to have the optimal algorithm, delete the line #define STOP_AT_DST_FOUND.
#ifdef STOP_AT_DST_FOUND
#define FAST_A_STAR_LOOP dst_node == nullptr && 
#else
#define FAST_A_STAR_LOOP
#endif

	struct Matrix {
		virtual float get(int x, int y) = 0;
		virtual size_t get_width() const = 0;
		virtual size_t get_height() const = 0;
	};

	enum class Direction {
		UP, DOWN, LEFT, RIGHT
	};

	typedef std::stack<Direction> Path;


	struct Vector2i {
		Vector2i(int x=0, int y=0) : x(x), y(y) {}
		bool operator==(const Vector2i &rval) const { return (x == rval.x && y == rval.y); }
		int x;
		int y;
	};

	struct Node {
		Vector2i coords;
		float local;
		float global;
		bool visited;
		Node *parent;
		Direction direction;

		Node(Vector2i coords=Vector2i(), Node *parent=nullptr, float local=3.402823466e+38F, float global=3.402823466e+38F, bool visited=false)
			: coords(coords), parent(parent), local(local), global(global), visited(visited) {}
	};

	struct NodeComparison {
		bool operator() (const Node *lhs, const Node *rhs) {
			return lhs->global > rhs->global;
		}
	};

	// matrix is a matrix representing the difficulty of each tile, from 0.0 (effortless) to 1.0 (obstacle).
	Path search(
			Matrix &matrix,
		   	Vector2i start,
		   	Vector2i end,
		   	unsigned int limit=50);

	bool in_bounds(Matrix &matrix, Vector2i coords);

	float distance(Vector2i na, Vector2i nb);
}
