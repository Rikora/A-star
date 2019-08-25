#include "AStar.hpp"
#include <fstream>
#include <string>
#include <algorithm>

namespace pf
{
	AStar::AStar() : 
	m_weightFactor(1),
	m_dimensions(0, 0),
	m_startPos(0, 0),
	m_targetPos(0, 0),
	m_size(0)
	{
		// No diagonal movement at this point
		m_directions = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } };
	}

	int AStar::findPath(const Vec2i& startPos, const Vec2i& targetPos)
	{
		m_startPos = startPos;
		m_targetPos = targetPos;

		if (m_grid.empty() || m_startPos == m_targetPos)
		{
			return -1;
		}

		// Init lists
		m_cameFrom.resize(m_size);
		m_closedList.resize(m_size, false);

		// Add the start node to the lists
		m_cameFrom[convertTo1D(m_startPos)].parent = m_startPos;
		m_openList.insert(Node(m_startPos, 0));

		uint fNew, gNew, hNew;
		Vec2i currentPos;

		while (!m_openList.empty())
		{
			// Find the node with the least f value
			auto curr_it = *m_openList.begin();
			currentPos = curr_it.pos;

			// If the current node is the target node, the search is complete
			if (currentPos == m_targetPos)
			{
				//return tracePath(cameFrom, pOutBuffer, nOutBufferSize);
			}

			// Remove the found node from the open list and then add it to the closed list
			m_openList.erase(curr_it);
			m_closedList[convertTo1D(currentPos)] = true;

			// Check the neighbors of the current node
			for (const auto& dir : m_directions)
			{
				const auto neighborPos = currentPos + dir;
				const auto neighborIndex = convertTo1D(neighborPos);

				// If the node is not valid or can't be passed or the node is already found
				if (!isValid(neighborPos) || isBlocked(neighborIndex) || m_closedList[neighborIndex] == true)
				{
					continue;
				}

				gNew = m_cameFrom[convertTo1D(currentPos)].g + 1;
				hNew = computeHeuristic(neighborPos);
				fNew = gNew + hNew;

				// If the node is not in the open list or the new f value is better
				if (m_cameFrom[neighborIndex].f == 0 || fNew < m_cameFrom[neighborIndex].f)
				{
					m_openList.insert(Node(neighborPos, fNew));
					m_cameFrom[neighborIndex] = { neighborPos, currentPos, fNew, gNew, hNew };
				}
			}
		}

		// If we could not find the path
		return -1;
	}

	void AStar::loadMap(const std::string& fileName)
	{
		std::ifstream file(fileName);
		
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				if (line.find('w') != std::string::npos)
				{
					line.erase(std::remove_if(line.begin(), line.end(), 
						[](unsigned char c) { return (c == 'w' || c == ':') ? true : false; }), line.end());
					m_dimensions.x = std::stoi(line);
				}
				else if (line.find('h') != std::string::npos)
				{
					line.erase(std::remove_if(line.begin(), line.end(),
						[](unsigned char c) { return (c == 'h' || c == ':') ? true : false; }), line.end());
					m_dimensions.y = std::stoi(line);
				}
				else
				{
					line.erase(std::remove(line.begin(), line.end(), ','), line.end());

					for (const auto& c : line)
					{
						m_grid.push_back(c - 48);
					}
				}
			}

			m_size = m_dimensions.x * m_dimensions.y;
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
	uint AStar::computeHeuristic(const Vec2i& pos)
	{
		return static_cast<uint>(m_weightFactor * (abs(pos.x - m_targetPos.x) + abs(pos.y - m_targetPos.y)));
	}

	// Returns a 1D index based on a 2D coordinate using row-major layout
	int AStar::convertTo1D(const Vec2i& pos)
	{
		return (pos.y * m_dimensions.x) + pos.x;
	}
}