#include "AStar.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

namespace pf
{
	AStar::AStar() : 
	m_weightFactor(1)
	{
	}

	void AStar::loadMap(const std::string& fileName)
	{
		std::ifstream file(fileName);
		
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				line.erase(std::remove(line.begin(), line.end(), ','), line.end());

				for (size_t i = 0; i < line.size(); ++i)
				{
					m_grid.push_back(line[i] - 48);
				}
			}

			file.close();
		}
	}

	bool AStar::isValid(const Vec2i& pos)
	{
		return (pos.x >= 0) && (pos.x < m_dimensions.x) && 
			   (pos.y >= 0) && (pos.y < m_dimensions.y);
	}

	bool AStar::isBlocked(int index)
	{
		return (m_grid[index] == 0);
	}

	// Manhattan distance for now...
	uint AStar::computeHeuristic(const Vec2i& pos, const Vec2i& targ)
	{
		return static_cast<uint>(m_weightFactor * (abs(pos.x - m_targetPos.x) + 
								 abs(pos.y - m_targetPos.y)));
	}

	// Returns a 1D index based on a 2D coordinate using row-major layout
	int AStar::convertTo1D(const Vec2i& pos)
	{
		return (pos.y * m_dimensions.x) + pos.x;
	}
}