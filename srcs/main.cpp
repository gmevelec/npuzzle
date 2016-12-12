#include <iostream>
#include <unordered_map>
#include "Parser.h"
#include "Exception.h"
#include "Node.h"
#include "implementation.h"

int		main(int ac, char **av)
{
	try
	{
		if (ac != 2)
		{
			throw Exception(-1, "Usage: ./n_puzzle puzzle", 0);
		}
		Graph	s = getNPuzzle(av[1]);
		Graph	g = generateGoal(s.size);
		if (!isSolvable(s))
		{
			throw Exception(8, "This puzzle is not solvable.", 2);
		}

		std::unordered_map<Node, Node> came_from;
		std::unordered_map<Node, float> cost_so_far;
		Node start(s.puzzle, s.size);
		start.h = 0;
		start.g = 0;
		start.f = 0;

		Node goal(g.puzzle, g.size);
		std::cout << start.size << std::endl;
		info_values info;

		a_star_search(start, goal, came_from, cost_so_far, info);

		std::vector<Node> path = reconstruct_path(start, goal, came_from);

		info.nbOfMoves = path.size() - 1;

		std::cout << "----------------------------" << std::endl;
		std::cout << "Total number of states ever selected in the opened set: " << info.nbStatesSelected << std::endl;
		std::cout << "Maximum number of states ever represented in memory at the same time: " << info.maxNbofStates << std::endl;
		std::cout << "Number of moves required: " << info.nbOfMoves << std::endl;
		std::cout << "----------------------------" << std::endl;
		// std::cout << "PATH = " << std::endl;
		// for (size_t i = 0; i < path.size(); ++i)
		// {
		// 	std::cout << path[i] << std::endl;
		// }
	}
	catch (std::exception const& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return (0);
}
