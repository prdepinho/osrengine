#include "astar.h"

std::stack<AStar::Direction> AStar::search(
		AStar::Matrix &matrix,
	   	AStar::Vector2i start,
	   	AStar::Vector2i end,
	   	unsigned int limit)
{
	// consider the destiny as not obstacle for path-finding purposes.
	bool obstacle_dst = false;
	if (matrix[end.x + end.y * matrix.width] == 1.f) {
		obstacle_dst = true;
		matrix[end.x + end.y * matrix.width] = 0.f;
	}

	std::map<AStar::Direction, std::tuple<int, int>> direction_mods = {
		{AStar::Direction::UP, std::make_tuple(0, -1)},
		{AStar::Direction::DOWN, std::make_tuple(0, +1)},
		{AStar::Direction::RIGHT, std::make_tuple(+1, 0)},
		{AStar::Direction::LEFT, std::make_tuple(-1, 0)}
	};

	std::vector<AStar::Direction> directions = {
		AStar::Direction::UP, AStar::Direction::DOWN, AStar::Direction::RIGHT, AStar::Direction::LEFT
	};

	std::vector<Node > search_grid(matrix.width * matrix.height);
	for (int x = 0; x < matrix.width; ++x) {
		for (int y = 0; y < matrix.height; ++y) {
			search_grid[y * matrix.width + x] = Node(AStar::Vector2i(x, y), nullptr);
		}
	}
	search_grid[start.y * matrix.width + start.x] = Node(start, nullptr, 0.0f, AStar::distance(start, end));

	std::priority_queue < Node*, std::vector<Node*>, NodeComparison > search_queue;
	search_queue.push(&search_grid[start.y * matrix.width + start.x]);

	Node *dst_node = nullptr;
	while (FAST_A_STAR_LOOP search_queue.size() >= 1) {
		Node *current = search_queue.top();
		if (current->local >= limit) {
			dst_node = current;
			break;
		}

		for (AStar::Direction &direction : directions) {
			int x = current->coords.x + std::get<0>(direction_mods[direction]);
			int y = current->coords.y + std::get<1>(direction_mods[direction]);
			if (in_bounds(matrix, Vector2i(x, y)) && !matrix[x + y * matrix.width] == 1.f) {
				AStar::Vector2i neighbor(x, y);
				Node *neighbor_node = &search_grid[neighbor.y * matrix.width + neighbor.x];

				if (!neighbor_node->visited && neighbor_node->local > current->local + 1) {
					neighbor_node->local = current->local + 1;
					neighbor_node->global = neighbor_node->local + AStar::distance(neighbor, end);
					neighbor_node->parent = current;
					neighbor_node->direction = direction;

					search_queue.push(neighbor_node);

					if (neighbor == end) {
						dst_node = neighbor_node;
					}
				}
			}
		}
		current->visited = true;
		search_queue.pop();
	}

	// return a stack of directions to follow.
	std::stack<AStar::Direction> path;
	if (dst_node != nullptr) {
		// remove destiny from path if it is an obstacle.
		if (obstacle_dst) {
			dst_node = dst_node->parent;
			matrix[end.x + end.y * matrix.width] = 1.f;
		}
		Node *a_star_node = dst_node;
		do {
			path.push(a_star_node->direction);
		} while ((a_star_node = a_star_node->parent) != nullptr);
	}

	return path;
}

inline float AStar::distance(AStar::Vector2i na, AStar::Vector2i nb) {
	float xl = std::abs((float)(na.x - nb.x));
	float yl = std::abs((float)(na.y - nb.y));
	return std::sqrt((xl * xl) + (yl * yl));
}

inline bool AStar::in_bounds(AStar::Matrix &matrix, AStar::Vector2i coords) {
	return (coords.x >= 0 && coords.y < matrix.width && coords.y >= 0 && coords.y < matrix.height);
}
