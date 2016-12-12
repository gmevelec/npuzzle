#pragma once

#include "Node.h"

#include <unordered_map>

#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define MAGENTA	"\x1B[35m"
#define CYAN	"\x1B[36m"
#define WHITE	"\x1B[37m"
#define RESET	"\033[0m"

struct info_values
{
	int					nbStatesSelected = 0;
	int					maxNbofStates = 0;
	int					nbOfMoves = 0;
	std::vector<Node>	path;
};

void	a_star_search(
	Node start,
	Node goal,
	std::unordered_map<Node, Node>& came_from,
	std::unordered_map<Node, float>& cost_so_far,
	info_values& info);

std::vector<Node>	reconstruct_path(Node start, Node goal, std::unordered_map<Node, Node>& came_from);
