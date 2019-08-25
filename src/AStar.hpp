#pragma once

#include <set>
#include <vector>
#include "Vec2i.hpp"

namespace pf
{
	using uint = unsigned int;

	struct Node
	{
		Node() : pos(0, 0), parent(-1, -1), f(0), g(0), h(0) {}
		Node(const Vec2i& pos, uint f) : pos(pos), parent(-1, 1), f(f), g(0), h(0) {}
		Node(const Vec2i& pos, const Vec2i& parent, uint f, uint g, uint h) : pos(pos), parent(parent), f(f), g(g), h(h) {}

		Vec2i pos;
		Vec2i parent;
		uint f;
		uint g;
		uint h;
	};

	// Required for std::set
	inline bool operator< (const Node& a, const Node& b) { return a.f < b.f; }

	class AStar
	{
	public:
		AStar();

		std::vector<Vec2i> findPath(const Vec2i& startPos, const Vec2i& targetPos, int weight = 1); // TODO: add heuristic function parameter
		void loadMap(const std::string& fileName);
		void setHeuristicWeight(int weight);
		int getHeuristicWeight() const;

	private:
		std::vector<Vec2i> buildPath() const;
		bool isValid(const Vec2i& pos) const;
		bool isBlocked(int index) const;
		uint computeHeuristic(const Vec2i& pos) const; // Change this later...
		int convertTo1D(const Vec2i& pos) const;

		int m_weight;
		int m_size;
		Vec2i m_dimensions;
		Vec2i m_startPos;
		Vec2i m_targetPos;
		std::set<Node> m_openList;
		std::vector<bool> m_closedList;
		std::vector<Node> m_cameFrom;
		std::vector<int> m_grid;
		std::vector<Vec2i> m_directions;
	};
}