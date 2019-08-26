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
		inline const Vec2i operator+ (const Vec2i& v) const { return Vec2i(x + v.x, y + v.y); }

		static Vec2i getDelta(const Vec2i& v1, const Vec2i& v2) { return Vec2i(abs(v1.x - v2.x), abs(v1.y - v2.y)); }

		int x;
		int y;
	};
}