#pragma once

namespace gameai {
	template<typename T> struct point2 {
		T x;
		T y;

		friend point2 operator-(const point2 &p, const point2 &q) {
			return point2<T>{p.x - q.x, p.y - q.y};
		}
	
		friend std::ostream & operator<<(std::ostream &s, const point2 &p) {
			return s << p.x << ',' << p.y;
		}
	
		friend std::istream & operator>>(std::istream &s, point2 &p) {
			char c;
			s >> p.x >> c >> p.y;
			if(c != ',') { s.setstate(std::ios_base::failbit); }
			return s;
		}
	};

	template<typename T> point2<T> abs(const point2<T> &p) {
		return point2<T>{std::abs(p.x), std::abs(p.y)};
	}

	template<typename T> float distance_squared(const point2<T> &p, const point2<T> &q) {
		auto dp = abs(p - q);
		return dp.x * dp.x + dp.y * dp.y;
	}
}
