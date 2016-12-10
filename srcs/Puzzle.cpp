#include "Puzzle.h"
#include "Exception.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Puzzle::Puzzle(std::string filename)
{
	this->fillPuzzle(filename);
	this->fillGoalPuzzle();
	this->fillLinePuzzle();
	if (!this->hasGoodValues())
	{
		throw Exception(7, "This puzzle has not good values.", 2);
	}
	else
	{
		std::cout << "GOOD VALUES" << std::endl;
	}
	if (!this->isSolvable())
	{
		throw Exception(8, "This puzzle is not solvable.", 2);
	}
	else
	{
		std::cout << "This puzzle is solvable." << std::endl;
	}
}

Puzzle::Puzzle(Puzzle const& src)
{
	*this = src;
}

Puzzle::~Puzzle()
{
}

void				Puzzle::fillPuzzle(std::string filename)
{
	std::ifstream		file(filename);
	std::string			line;
	std::vector<int>	intLine;
	int 				currentline = 0;

	this->size = -1;
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
		int nbToken = 0;
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
			if (this->size == -1)
			{
				if (intLine.size() == 1)
				{
					this->size = intLine[0];
					if (this->size > 46340)
					{
						throw Exception(1, "Size is too high", 2);
					}
					if (this->size < 3)
					{
						throw Exception(2, "Size is too low", 2);
					}
					this->puzzle = new (std::nothrow) int*[this->size];
					if (this->puzzle == nullptr)
					{
						throw Exception(3, "Memory could not be allocated", 2);
					}
					for (int i = 0; i < this->size; i++)
					{
						this->puzzle[i] = new (std::nothrow) int[this->size];
						if (this->puzzle[i] == nullptr)
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
				if (intLine.size() == this->size && currentline != this->size)
				{
					for (int i = 0; i < this->size; i++)
					{
						this->puzzle[currentline][i] = intLine[i];
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
	if (this->size == -1)
	{
		throw Exception(6, "You must enter a size", 2);
	}
	this->sizeLine = this->size * this->size;
}

void				Puzzle::fillGoalPuzzle()
{
	this->goal = new int*[this->size];
	if (this->goal == nullptr)
	{
		throw Exception(3, "Memory could not be allocated", 2);
	}
	for (int i = 0; i < this->size; i++)
	{
		this->goal[i] = new (std::nothrow) int[this->size];
		if (this->goal[i] == nullptr)
		{
			throw Exception(3, "Memory could not be allocated", 2);
		}
	}

	int 		count = 1;
	char		dir = 'r';

	int n = 0;
	int x = 0;
	int y = 0;
	while (count < this->sizeLine)
	{
		goal[y][x] = count;
		dir = getDir(x, y, this->size - 1, dir, n);
		incPosXY(x, y, dir);
		count++;
	}
	goal[y][x] = 0;
}

void				Puzzle::fillLinePuzzle()
{
	this->linePuzzle = new int[this->sizeLine];
	int n = 0;
	int x = 0;
	int y = 0;
	int count = 0;
	char dir = 'r';
	while (count < this->sizeLine)
	{
		linePuzzle[count] = this->puzzle[y][x];
		dir = getDir(x, y, this->size - 1, dir, n);
		incPosXY(x, y, dir);
		count++;
	}
	std::cout << "Puzzle = ";
	for (int i = 0; i < this->sizeLine; i++)
	{
		std::cout << this->linePuzzle[i] << " ";
	}
	std::cout << std::endl;
}

void				Puzzle::printPuzzle()
{
	int i = 0;
	while (i < this->size)
	{
		int j = 0;
		while (j < this->size)
		{
			if (j != this->size - 1)
				std::cout << this->puzzle[i][j] << " ";
			else
				std::cout << this->puzzle[i][j];
			j++;
		}
		std::cout << std::endl;
		i++;
	}
}

bool				Puzzle::hasGoodValues()
{
	int 	tmp[this->sizeLine];
	int		tmp2[this->sizeLine];

	for (int i = 0; i < this->sizeLine; ++i)
	{
		tmp[i] = i;
	}
	for (int i = 0; i < this->sizeLine; ++i)
	{
		tmp2[i] = this->linePuzzle[i];
	}
	std::sort(tmp2, tmp2 + this->sizeLine);
	return (std::equal(tmp, tmp + this->sizeLine ,tmp2));
}

bool				Puzzle::isSolvable()
{
	int inversions = 0;

	for (int i = 0; i < this->sizeLine; i++)
	{
		if (this->linePuzzle[i] != 0)
		{
			for (int j = i + 1; j < this->sizeLine; j++)
			{
				if ((this->linePuzzle[i] > this->linePuzzle[j]) && this->linePuzzle[j] != 0)
				{
					inversions++;
				}
			}
		}
	}
	return (!(inversions % 2));
}

Puzzle&				Puzzle::operator=(Puzzle const & rhs)
{
	this->size = rhs.size;
	this->puzzle = rhs.puzzle;
	this->goal = rhs.goal;
	this->linePuzzle = rhs.linePuzzle;
	this->sizeLine = rhs.sizeLine;
	return (*this);
}
