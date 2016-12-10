#pragma once

class Puzzle
{
public:
	Puzzle(std::string filename);
	Puzzle(Puzzle const & src);
	virtual ~Puzzle();

	void			printPuzzle();
	bool			hasGoodValues();
	bool			isSolvable();
	void			fillPuzzle(std::string filename);
	void			fillGoalPuzzle();
	void			fillLinePuzzle();

	Puzzle&			operator=(Puzzle const & rhs);

	int		size;
	int**	puzzle;
	int**	goal;

	int*	linePuzzle;
	int 	sizeLine;
};
