# A* algorithm
Fast implementation of the A* path-finding algorithm with C++ (C++14). 

## Usage example

```c++
#include "AStar.hpp"
#include <iostream>

int main()
{
    pf::AStar astar;

    astar.loadMap("maps/map01.txt");
    astar.setDiagonalMovement(true);
    auto path = astar.findPath(pf::Vec2i(0, 0), pf::Vec2i(10, 7), pf::heuristic::euclidean, 1);

    for (const auto& coord : path) 
    {
        std::cout << coord.x << "," << coord.y << "\n";
    }

    system("pause");
    return 0;
}
```

## Resources

## License

This library is licensed under the MIT License, see LICENSE for more information.
