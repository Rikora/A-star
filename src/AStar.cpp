#include "AStar.hpp"
#include <fstream>
#include <string>
#include <algorithm>

using namespace std::placeholders;

namespace pf
{
	AStar::AStar() : 
	m_weight(1),
	m_dimensions(0, 0),
	m_startPos(0, 0),
	m_targetPos(0, 0),
	m_size(0),
	m_nrOfDirections(4)
	{
		m_directions = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 },
						 { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };
	}

	std::vector<Vec2i> AStar::findPath(const Vec2i& startPos, const Vec2i& targetPos, HeuristicFunction heuristicFunc, int weight)
	{
		m_startPos = startPos;
		m_targetPos = targetPos;
		m_weight = weight;
		m_heuristic = std::bind(heuristicFunc, _1, _2, _3);
		m_cameFrom.resize(m_size);
		m_closedList.resize(m_size, false);

		m_cameFrom[convertTo1D(m_startPos)].parent = m_startPos;
		m_openList.push(Node(m_startPos, 0));

		uint fNew, gNew, hNew;
		Vec2i currentPos;

		while (!m_openList.empty())
		{
			// Get the node with the least f value
			currentPos = m_openList.top().pos;

			if (currentPos == m_targetPos)
			{
				break;
			}

			m_openList.pop();
			m_closedList[convertTo1D(currentPos)] = true;

			// Check the neighbors of the current node
			for (uint i = 0; i < m_nrOfDirections; ++i)
			{
				const auto neighborPos = currentPos + m_directions[i];
				const auto neighborIndex = convertTo1D(neighborPos);

				if (!isValid(neighborPos) || isBlocked(neighborIndex) || m_closedList[neighborIndex] == true)
				{
					continue;
				}

				gNew = m_cameFrom[convertTo1D(currentPos)].g + 1;
				hNew = m_heuristic(neighborPos, m_targetPos, m_weight);
				fNew = gNew + hNew;

				if (m_cameFrom[neighborIndex].f == 0 || fNew < m_cameFrom[neighborIndex].f)
				{
					m_openList.push(Node(neighborPos, fNew));
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

	void AStar::setDiagonalMovement(bool enable)
	{
		m_nrOfDirections = (enable) ? 8 : 4;
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

	// Returns a 1D index based on a 2D coordinate using row-major layout
	int AStar::convertTo1D(const Vec2i& pos) const
	{
		return (pos.y * m_dimensions.x) + pos.x;
	}

	uint heuristic::manhattan(const Vec2i& v1, const Vec2i& v2, int weight)
	{
		const auto delta = Vec2i::getDelta(v1, v2);
		return static_cast<uint>(weight * (delta.x + delta.y));
	}

	uint heuristic::euclidean(const Vec2i& v1, const Vec2i& v2, int weight)
	{
		const auto delta = Vec2i::getDelta(v1, v2);
		return static_cast<uint>(weight * sqrt((delta.x * delta.x) + (delta.y * delta.y)));
	}
}

