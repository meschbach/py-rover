#ifndef _pathfinder_point_h_
#define _pathfinder_point_h_
/*
 *
 */
#include <algorithm>
#include <iostream>

namespace rover {

/***************************************
 * Collapse to a single dimension
 **************************************/
template<typename DimensionType = size_t>
inline DimensionType horizontal_first_dimension_point( DimensionType x, DimensionType y, DimensionType width ){
    return x + ( y * width );
}

/***************************************
 * 2D Point
 **************************************/
typedef std::pair<int,int> Point;

inline std::ostream& operator<<( std::ostream& out, const Point& p ) {
	out << p.first << "," << p.second;
	return out;
}

inline Point operator+( const Point lhs, const Point rhs ){
    Point neighbor = { lhs.first + rhs.first, lhs.second + rhs.second };
    return neighbor;
}

inline bool operator<( const Point lhs, const Point rhs ){
    return (lhs.first < rhs.first)
        && (lhs.second < rhs.second);
}

struct PointLessThanComparator : public std::binary_function<Point,Point,bool>{
    int width;

    PointLessThanComparator( int aWidth ) : width(aWidth) {}

    bool operator()(const Point &lhs, const Point &rhs ) const {
        auto lhsPoint = horizontal_first_dimension_point(lhs.first, lhs.second, width) ;
        auto rhsPoint = horizontal_first_dimension_point(rhs.first, rhs.second, width) ;
        return lhsPoint < rhsPoint;
    }
};

} //::rover

#endif /* _pathfinder_point_h_ */