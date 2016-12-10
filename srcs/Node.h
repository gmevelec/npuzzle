#pragma once

#include <vector>
#include <iostream>

class Node
{
public:
	Node(int**, int);
	Node(Node const& src);
	~Node();

	std::vector<std::vector<int> >	puzzle;
	int								size;

	float							h;
	float							g;
	float							f;

	/*
	** Operator overload
	*/
	Node&	operator=(Node const& rhs);
};

bool			operator==(Node const& lhs, Node const& rhs);
bool			operator!=(Node const& lhs, Node const& rhs);
bool			operator<(Node const& lhs, Node const& rhs);
std::ostream&	operator<<(std::ostream& os, const Node& node);


namespace std {
	template <>
	struct hash<Node> {
		std::size_t operator()(const Node& location) const
		{
			std::size_t seed = location.puzzle[0].size();
			for (std::size_t i = 0; i < location.puzzle.size(); ++i)
			{
				for(auto& n : location.puzzle[i])
				{
				    seed ^= n + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				}
			}
			return seed;
		}
	};
}
