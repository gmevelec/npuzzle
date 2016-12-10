
// struct SquareGrid {
//   typedef tuple<int,int> Location;
//   static array<Location, 4> DIRS;

//   int width, height;
//   unordered_set<Location> walls;

//   SquareGrid(int width_, int height_)
//      : width(width_), height(height_) {}

//   inline bool in_bounds(Location id) const {
//     int x, y;
//     tie (x, y) = id;
//     return 0 <= x && x < width && 0 <= y && y < height;
//   }

//   inline bool passable(Location id) const {
//     return !walls.count(id);
//   }

//   vector<Location> neighbors(Location id) const {
//     int x, y, dx, dy;
//     tie (x, y) = id;
//     vector<Location> results;

//     for (auto dir : DIRS) {
//       tie (dx, dy) = dir;
//       Location next(x + dx, y + dy);
//       if (in_bounds(next) && passable(next)) {
//         results.push_back(next);
//       }
//     }

//     if ((x + y) % 2 == 0) {
//       // aesthetic improvement on square grids
//       std::reverse(results.begin(), results.end());
//     }

//     return results;
//   }
// };

// struct GridWithWeights: SquareGrid {
//   unordered_set<Location> forests;
//   GridWithWeights(int w, int h): SquareGrid(w, h) {}
//   double cost(Location from_node, Location to_node) const { 
//     return forests.count(to_node) ? 5 : 1; 
//   }
// };

// int main() {
//   GridWithWeights grid = make_diagram4(); DONE
//   SquareGrid::Location start{1, 4}; DONE
//   SquareGrid::Location goal{8, 5}; DONE
//   unordered_map<SquareGrid::Location, SquareGrid::Location> came_from;
//   unordered_map<SquareGrid::Location, double> cost_so_far;
//   a_star_search(grid, start, goal, came_from, cost_so_far);
//   draw_grid(grid, 2, nullptr, &came_from);
//   std::cout << std::endl;
//   draw_grid(grid, 3, &cost_so_far, nullptr);
//   std::cout << std::endl;
//   vector<SquareGrid::Location> path = reconstruct_path(start, goal, came_from);
//   draw_grid(grid, 3, nullptr, nullptr, &path);
// }

#include "implementation.h"
#include <queue>
#include <map>
#include <iostream>

template<typename T, typename priority_t>
struct PriorityQueue
{
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement> > elements;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void put(T item, priority_t priority)
	{
		elements.emplace(priority, item);
	}

	inline T get()
	{
		T bestItem = elements.top().second;
		elements.pop();
		return (bestItem);
	}
};

std::vector<Node>	reconstruct_path(Node start, Node goal, std::unordered_map<Node, Node>& came_from)
{
	std::vector<Node>	path;
	Node 				current = goal;

	path.push_back(current);
	while (current != start)
	{
		current = came_from.at(current);
		path.push_back(current);
	}
	std::reverse(path.begin(), path.end());
	return (path);
}


void				getZero(Node puzzle, int& x, int& y)
{
	for (int i = 0; i < puzzle.size; ++i)
	{
		for (int j = 0; j < puzzle.size; ++j)
		{
			if (puzzle.puzzle[i][j] == 0)
			{
				x = j;
				y = i;
				return ;
			}
		}
	}
}

std::vector<Node> getNeighbors(Node puzzle)
{
	int x, y;
	std::vector<Node> neighbors;
	getZero(puzzle, x, y);

	if (x < puzzle.size - 1)
	{
		Node tmp = puzzle;
		tmp.puzzle[y][x] = tmp.puzzle[y][x + 1];
		tmp.puzzle[y][x + 1] = 0;
		neighbors.push_back(tmp);
	}
	if (x > 0)
	{
		Node tmp = puzzle;
		tmp.puzzle[y][x] = tmp.puzzle[y][x - 1];
		tmp.puzzle[y][x - 1] = 0;
		neighbors.push_back(tmp);

	}

	if (y < puzzle.size - 1)
	{
		Node tmp = puzzle;
		tmp.puzzle[y][x] = tmp.puzzle[y + 1][x];
		tmp.puzzle[y + 1][x] = 0;
		neighbors.push_back(tmp);

	}
	if (y > 0)
	{
		Node tmp = puzzle;
		tmp.puzzle[y][x] = tmp.puzzle[y - 1][x];
		tmp.puzzle[y - 1][x] = 0;
		neighbors.push_back(tmp);

	}
	return (neighbors);
}

int 	heuristic(Node puzzle, Node goal)
{
	int n = 0;
	for (int i = 0; i < puzzle.size; ++i)
	{
		for (int j = 0; j < puzzle.size; ++j)
		{
			if (puzzle.puzzle[i][j] != goal.puzzle[i][j] && puzzle.puzzle[i][j] != 0)
			{
				n++;
			}
		}
	}
	return (n);
}

void 	getRowColByValue(Node puzzle, int& expectedRow, int& expectedColumn, int value)
{
	for (int i = 0; i < puzzle.size; ++i)
	{
		for (int j = 0; j < puzzle.size; ++j)
		{
			if (puzzle.puzzle[i][j] == value)
			{
				expectedRow = i;
				expectedColumn = j;
				return ;
			}
		}
	}
}


int 	manhattan(Node puzzle, Node goal)
{
	int heuristic = 0;

	for (int i = 0; i < puzzle.size; ++i)
	{
		for (int j = 0; j < puzzle.size; ++j)
		{
			int value = puzzle.puzzle[i][j];
			if (value == 0)
			{
				continue;
			}
			int expectedRow;
			int expectedColumn;

			getRowColByValue(goal, expectedRow, expectedColumn, value);

			int diff = abs(i - expectedRow) + abs(j - expectedColumn);
			heuristic += diff;
		}
	}
	return (heuristic);
}


int 	linearVerticalConflict(Node puzzle)
{
	int linearConflict = 0;

	for (int i = 0; i < puzzle.size; ++i)
	{
		int max = -1;
		for (int j = 0; j < puzzle.size; ++j)
		{
			int value = puzzle.puzzle[i][j];
			if (value != 0 && (value - 1) / puzzle.size == i)
			{
				if (value > max)
				{
					max = value;
				}
				else
				{
					linearConflict += 2;
				}
			}
		}
	}
	return (linearConflict);
}

int 	linearHorizontalConflict(Node puzzle)
{
	int linearConflict = 0;

	for (int j = 0; j < puzzle.size; ++j)
	{
		int max = -1;
		for (int i = 0; i < puzzle.size; ++i)
		{
			int value = puzzle.puzzle[i][j];
			if (value != 0 && value % puzzle.size == j + 1)
			{
				if (value > max)
				{
					max = value;
				}
				else
				{
					linearConflict += 2;
				}
			}
		}
	}
	return (linearConflict);
}

int 	linearConflict(Node puzzle, Node goal)
{
	int heuristic = manhattan(puzzle, goal);

	heuristic += linearVerticalConflict(puzzle);
	heuristic += linearHorizontalConflict(puzzle);

	return (heuristic);
}

void	a_star_search(
	Node start,
	Node goal,
	std::unordered_map<Node, Node>& came_from,
	std::unordered_map<Node, float>& cost_so_far)
{
	PriorityQueue<Node, float> open;

	std::cout << YELLOW;
	std::cout << start;
	std::cout << RESET;
	open.put(start, 0);
	came_from.emplace(Node(start), start);
	cost_so_far[start] = 0;

	std::vector<Node> neighbors;
	while (!open.empty())
	{
		auto current = open.get();
		if (current == goal)
		{
			std::cout << "FINISHED" << std::endl;
			break;
		}
		neighbors.clear();
		neighbors = getNeighbors(current);
		for (auto next : neighbors)
		{
			float new_cost = cost_so_far[current] + 1;
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next])
			{
				cost_so_far[next] = new_cost;
				next.h = linearConflict(next, goal);
				next.g = new_cost;
				next.f = next.g + next.h;
				open.put(next, next.f);
				came_from.emplace(Node(next), current);
			}
		}
	}
}
