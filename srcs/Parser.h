#pragma once

#include <string>

struct Graph
{
	int 	size;
	int**	puzzle;

};

std::ostream& operator << (std::ostream& os, const Graph& rhs);

Graph	getNPuzzle(std::string filename);
Graph	generateGoal(int size);
bool	isSolvable(Graph puzzle);

