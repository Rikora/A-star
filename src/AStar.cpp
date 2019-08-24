#include "AStar.hpp"

namespace pf
{
	AStar::AStar() : 
	m_weightFactor(1)
	{
	}

	void AStar::loadMap(const std::string& fileName)
	{
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