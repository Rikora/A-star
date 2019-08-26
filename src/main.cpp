#include "AStar.hpp"
#include <iostream>

int main()
{
	pf::AStar astar;

	astar.loadMap("maps/map01.txt");
	astar.setDiagonalMovement(true);
	auto path = astar.findPath(pf::Vec2i(0, 0), pf::Vec2i(10, 7), pf::heuristic::euclidean, 10);

	for (const auto& coord : path) 
	{
		std::cout << coord.x << "," << coord.y << "\n";
	}

	system("pause");
	return 0;
}