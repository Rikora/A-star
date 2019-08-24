#pragma once

#include "Vec2i.hpp"
#include <set>
#include <vector>

namespace pf
{
	using uint = unsigned int;

	struct Node
	{
		Node() : parent(-1, -1), f(0), g(0), h(0) {}
		Node(const Vec2i& parent, uint f, uint g, uint h) : parent(parent), f(f), g(g), h(h) {}

		Vec2i parent;
		uint f;
		uint g;
		uint h;
	};

	// Required for std::set
	bool operator< (const Node& a, const Node& b) { return a.f < b.f; }

	class AStar
	{
	public:
		AStar();

		void loadMap(const std::string& fileName);

	private:
		bool isValid(const Vec2i& pos);
		bool isBlocked(int index);
		uint computeHeuristic(const Vec2i& pos, const Vec2i& targ); // Change this
		int convertTo1D(const Vec2i& pos);

		int m_weightFactor;
		int m_size;
		Vec2i m_dimensions;
		Vec2i m_startPos;
		Vec2i m_targetPos;
		std::set<Node> m_openList;
		std::vector<bool> m_closedList;
		std::vector<int> m_grid;
		std::vector<Vec2i> m_directions;
	};
}