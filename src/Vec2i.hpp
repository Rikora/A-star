#pragma once

namespace pf
{
	// Utility class to represent a 2D vector
	class Vec2i
	{
	public:
		inline Vec2i(int x, int y) : x(x), y(y) {}
		inline Vec2i() = default;

		inline bool operator== (const Vec2i& v) const { return (x == v.x) && (y == v.y); }
		//inline bool operator< (const Vec2i& v) const { return (x < v.x) || ((x == v.x) && (y < v.y)); }
		inline const Vec2i operator+ (const Vec2i& v) const { return Vec2i(x + v.x, y + v.y); }

		int x;
		int y;
	};
}