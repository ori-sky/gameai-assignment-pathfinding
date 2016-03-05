#pragma once

namespace gameai {
	template<typename T> struct point2 {
		T x;
		T y;
	
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
}
