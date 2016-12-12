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
	std::unordered_map<Node, float>& cost_so_far,
	info_values& info)
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
		info.nbStatesSelected++;
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
				info.maxNbofStates++;
				next.h = linearConflict(next, goal);
				next.g = new_cost;
				next.f = next.g + next.h;
				open.put(next, next.f);
				info.maxNbofStates++;
				came_from.emplace(Node(next), current);
			}
		}
	}
}
