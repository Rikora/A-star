#include "AStar.hpp"
#include <iostream>

int main()
{
	pf::AStar astar;

	astar.loadMap("maps/map01.txt");
	auto v = astar.findPath({0, 0}, {4, 3}, pf::heuristic::manhattan, 1);

	for (const auto& coord : v) 
	{
		std::cout << coord.x << "," << coord.y << "\n";
	}

	system("pause");
	return 0;
}