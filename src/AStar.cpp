#include "AStar.hpp"
#include <fstream>
#include <string>
#include <algorithm>

namespace pf
{
	AStar::AStar() : 
	m_weight(1),
	m_dimensions(0, 0),
	m_startPos(0, 0),
	m_targetPos(0, 0),
	m_size(0)
	{
		// No diagonal movement at this point
		m_directions = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } };
	}

	std::vector<Vec2i> AStar::findPath(const Vec2i& startPos, const Vec2i& targetPos, int weight)
	{
		// Init variables
		m_startPos = startPos;
		m_targetPos = targetPos;
		m_weight = weight;
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

			// If the current node is the target node, we can start building the path
			if (currentPos == m_targetPos)
			{
				break;
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

		return buildPath();
	}

	std::vector<Vec2i> AStar::buildPath() const
	{
		std::vector<Vec2i> path;
		auto currentPos = m_targetPos;
		auto currentIndex = convertTo1D(currentPos);

		// Build the path 
		while (!(m_cameFrom[currentIndex].parent == currentPos))
		{
			path.push_back(currentPos);
			currentPos = m_cameFrom[currentIndex].parent;
			currentIndex = convertTo1D(currentPos);
		}

		std::reverse(path.begin(), path.end());

		return path;
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

	bool AStar::isValid(const Vec2i& pos) const
	{
		return (pos.x >= 0) && (pos.x < m_dimensions.x) && 
			   (pos.y >= 0) && (pos.y < m_dimensions.y);
	}

	bool AStar::isBlocked(int index) const
	{
		return (m_grid[index] == 0);
	}

	// Manhattan distance for now...
	uint AStar::computeHeuristic(const Vec2i& pos) const
	{
		return static_cast<uint>(m_weight * (abs(pos.x - m_targetPos.x) + abs(pos.y - m_targetPos.y)));
	}

	// Returns a 1D index based on a 2D coordinate using row-major layout
	int AStar::convertTo1D(const Vec2i& pos) const
	{
		return (pos.y * m_dimensions.x) + pos.x;
	}

	void AStar::setHeuristicWeight(int weight)
	{
		m_weight = weight;
	}

	int AStar::getHeuristicWeight() const
	{
		return m_weight;
	}
}