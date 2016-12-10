#include "Node.h"
/*
**	CONSTRUCTORS
*/
Node::Node(int** puzzle, int size)
{
	this->size = size;
	this->puzzle.resize(size);
	for (int i = 0; i < size; ++i)
	{
		this->puzzle[i].resize(size);
		for (int j = 0; j < size; ++j)
		{
			this->puzzle[i][j] = puzzle[i][j];
		}
	}
}

Node::Node(Node const& src)
{
	*this = src;
}

/*
** DESTRUCTOR
*/
Node::~Node()
{
}

// std::vector<Node>	getNeighbors(Node current)
// {
// 	std::vector<Node> 	neighbors;
// 	int					x;		
// 	int					y;

// 	for (int i = 0; i < current.size(); ++i)
// 	{
// 		for (int j = 0; j < current[i].size(); ++j)
// 		{
// 			x = i;
// 			y = j;
// 			break;
// 		}
// 	}
// }

/*
** OPERATOR OVERLOAD
*/
Node&		Node::operator=(Node const& rhs)
{
	this->puzzle = rhs.puzzle;
	this->size = rhs.size;
	this->f = rhs.f;
	this->g = rhs.g;
	this->h = rhs.h;
	return (*this);
}

bool	operator==(Node const& lhs, Node const& rhs)
{
	if (lhs.size != rhs.size)
		return (false);
	for (std::size_t i = 0; i < lhs.puzzle.size(); ++i)
	{
		for (std::size_t j = 0; j < lhs.puzzle.size(); ++j)
			if (lhs.puzzle[i][j] != rhs.puzzle[i][j])
				return (false);
	}
	return (true);
}

bool	operator!=(Node const& lhs, Node const& rhs)
{
	if (lhs.size != rhs.size)
		return (true);
	for (std::size_t i = 0; i < lhs.puzzle.size(); ++i)
	{
		for (std::size_t j = 0; j < lhs.puzzle.size(); ++j)
			if (lhs.puzzle[i][j] != rhs.puzzle[i][j])
				return (true);
	}
	return (false);
}

bool		operator<(Node const& lhs, Node const& rhs)
{
	return (lhs.h < rhs.h);
}

std::ostream&	operator<<(std::ostream& os, const Node& node)
{
	os << "Size = " << node.size << "	H = " << node.h << std::endl;
	for (std::size_t i = 0; i < node.puzzle.size(); ++i)
	{
		for (std::size_t j = 0; j < node.puzzle.size(); ++j)
		{
			os << node.puzzle[i][j] << " ";
		}
		os << std::endl;
	}
	os << "-------------------------" << std::endl;
	return (os);
}
