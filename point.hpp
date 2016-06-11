#ifndef _pathfinder_point_h_
#define _pathfinder_point_h_
/*
 *
 */
#include <algorithm>
#include <iostream>

namespace rover {
/***************************************
 * 2D Point
 **************************************/
typedef std::pair<int,int> Point;

inline std::ostream& operator<<( std::ostream& out, Point& p ) {
	out << p.first << "," << p.second;
	return out;
}

inline Point operator+( const Point lhs, const Point rhs ){
    Point neighbor = { lhs.first + rhs.first, lhs.second + rhs.second };
    return neighbor;
}

} //::rover

#endif /* _pathfinder_point_h_ */