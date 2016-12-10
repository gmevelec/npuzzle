#include "Parser.h"
#include "Exception.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

static void			incPosXY(int& x, int& y, char dir)
{
	if (dir == 'r')
	{
		x++;
	}
	else if (dir == 'd')
	{
		y++;
	}
	else if (dir == 'l')
	{
		x--;
	}
	else if (dir == 'u')
	{
		y--;
	}
}

static char			getDir(int x, int y, int size, char dir, int &n)
{
	char retDir;

	if (dir == 'r' && x == size - n)
	{
		retDir = 'd';
	}
	else if (dir == 'd' && y == size - n)
	{
		retDir = 'l';
	}
	else if (dir == 'l' && x == 0 + n)
	{
		retDir = 'u';
		n++;
	}
	else if (dir == 'u' && y == 0 + n)
	{
		retDir = 'r';
	}
	else
	{
		retDir = dir;
	}
	return (retDir);
}

Graph	getNPuzzle(std::string filename)
{
	Graph puzzle;
	std::ifstream		file(filename);
	std::string			line;
	std::vector<int>	intLine;
	int 				currentline = 0;

	if (!file.is_open())
	{
		throw Exception(0, "Can't open file " + filename, 2);
	}
	puzzle.size = -1;
	while (std::getline(file, line))
	{
		intLine.clear();
		int i = 0;
		while (line[i])
		{
			if (line[i] == '\t')
				line[i] = ' ';
			i++;
		}
		std::istringstream iss(line);
		std::string token;
		while (std::getline(iss, token, ' '))
		{
			if (token[0] == '#')
				break;
			if (token[0] != '\0')
			{
				intLine.push_back(std::stoi(token));
			}
		}
		if (!intLine.empty())
		{
			if (puzzle.size == -1)
			{
				if (intLine.size() == 1)
				{
					puzzle.size = intLine[0];
					if (puzzle.size > 46340)
					{
						throw Exception(1, "Size is too high", 2);
					}
					if (puzzle.size < 3)
					{
						throw Exception(2, "Size is too low", 2);
					}
					puzzle.puzzle = new (std::nothrow) int*[puzzle.size];
					if (puzzle.puzzle == nullptr)
					{
						throw Exception(3, "Memory could not be allocated", 2);
					}
					for (int i = 0; i < puzzle.size; i++)
					{
						puzzle.puzzle[i] = new (std::nothrow) int[puzzle.size];
						if (puzzle.puzzle[i] == nullptr)
						{
							throw Exception(3, "Memory could not be allocated", 2);
						}
					}
				}
				else
				{
					throw Exception(4, "Size must be the only value on the line", 2);
				}
			}
			else
			{
				if ((int)intLine.size() == puzzle.size && currentline != puzzle.size)
				{
					for (int i = 0; i < puzzle.size; i++)
					{
						puzzle.puzzle[currentline][i] = intLine[i];
					}
					currentline++;
				}
				else
				{
					throw Exception(5, "The number of values is different of the size expected", 2);
				}
			}
		}
	}
	if (puzzle.size == -1)
	{
		throw Exception(6, "You must enter a size", 2);
	}
	// puzzle.sizeLine = puzzle.size * puzzle.size;
	return (puzzle);
}

Graph	generateGoal(int size)
{
	int sizeLine = size * size;
	Graph goal;
	goal.size = size;
	goal.puzzle = new int*[size];
	if (goal.puzzle == nullptr)
	{
		throw Exception(3, "Memory could not be allocated", 2);
	}
	for (int i = 0; i < size; i++)
	{
		goal.puzzle[i] = new (std::nothrow) int[size];
		if (goal.puzzle[i] == nullptr)
		{
			throw Exception(3, "Memory could not be allocated", 2);
		}
	}

	int 		count = 1;
	char		dir = 'r';

	int n = 0;
	int x = 0;
	int y = 0;
	while (count < sizeLine)
	{
		goal.puzzle[y][x] = count;
		dir = getDir(x, y, size - 1, dir, n);
		incPosXY(x, y, dir);
		count++;
	}
	goal.puzzle[y][x] = 0;
	return (goal);
}

std::ostream& operator << (std::ostream& os, const Graph& rhs)
{
    os << "size = " << rhs.size << std::endl;
	int i = 0;
	while (i < rhs.size)
	{
		int j = 0;
		while (j < rhs.size)
		{
			if (j != rhs.size - 1)
				os << rhs.puzzle[i][j] << " ";
			else
				os << rhs.puzzle[i][j];
			j++;
		}
		os << std::endl;
		i++;
	}
    return os;
}

bool		hasGoodValues(int* linePuzzle, int sizeLine)
{
	int 	tmp[sizeLine];
	int		tmp2[sizeLine];

	for (int i = 0; i < sizeLine; ++i)
	{
		tmp[i] = i;
	}
	for (int i = 0; i < sizeLine; ++i)
	{
		tmp2[i] = linePuzzle[i];
	}
	std::sort(tmp2, tmp2 + sizeLine);
	return (std::equal(tmp, tmp + sizeLine ,tmp2));
}


bool		isSolvable(Graph puzzle)
{
	int sizeLine = puzzle.size * puzzle.size;
	int inversions = 0;

	int* linePuzzle = new int[sizeLine];
	int n = 0;
	int x = 0;
	int y = 0;
	int count = 0;
	char dir = 'r';
	while (count < sizeLine)
	{
		linePuzzle[count] = puzzle.puzzle[y][x];
		dir = getDir(x, y, puzzle.size - 1, dir, n);
		incPosXY(x, y, dir);
		count++;
	}
	if (!hasGoodValues(linePuzzle, sizeLine))
	{
		throw Exception(7, "This puzzle has not good values.", 2);
	}
	for (int i = 0; i < sizeLine; i++)
	{
		if (linePuzzle[i] != 0)
		{
			for (int j = i + 1; j < sizeLine; j++)
			{
				if ((linePuzzle[i] > linePuzzle[j]) && linePuzzle[j] != 0)
				{
					inversions++;
				}
			}
		}
	}
	return (!(inversions % 2));
}