#pragma once

#include <queue>
#include <vector>
#include <functional>
#include "Vec2i.hpp"

namespace pf
{
	using uint = unsigned int;
	using HeuristicFunction = std::function<uint(const Vec2i&, const Vec2i&, int)>;

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

	// Reverse std::priority_queue to get the smallest element on top
	inline bool operator< (const Node& a, const Node& b) { return a.f < b.f; }

	class AStar
	{
	public:
		AStar();

		std::vector<Vec2i> findPath(const Vec2i& startPos, const Vec2i& targetPos, HeuristicFunction heuristicFunc, int weight = 1);
		void loadMap(const std::string& fileName);
		void setDiagonalMovement(bool enable);

	private:
		std::vector<Vec2i> buildPath() const;
		bool isValid(const Vec2i& pos) const;
		bool isBlocked(int index) const;
		int convertTo1D(const Vec2i& pos) const;

		int m_weight;
		int m_size;
		uint m_nrOfDirections;
		Vec2i m_dimensions;
		Vec2i m_startPos;
		Vec2i m_targetPos;
		std::priority_queue<Node> m_openList;
		std::vector<bool> m_closedList;
		std::vector<Node> m_cameFrom;
		std::vector<int> m_grid;
		std::vector<Vec2i> m_directions;
		HeuristicFunction m_heuristic;
	};

	namespace heuristic
	{
		uint manhattan(const Vec2i& v1, const Vec2i& v2, int weight);
		uint euclidean(const Vec2i& v1, const Vec2i& v2, int weight);
	}
}