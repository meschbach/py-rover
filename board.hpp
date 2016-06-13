#ifndef _pathfinder_board_h_
#define _pathfinder_board_h_
/*
 *
 */
#include<iomanip>
#include<map>
#include<vector>
#include<algorithm>
#include<sstream>

#include "point.hpp"

namespace rover {

/***************************************
 * Verify Domain
 **************************************/
template<typename T>
inline void assert_in_range( const T low, const T high, const T actual, const char* name ) {
    if( low <= actual && actual < high ){
    }else{
        std::stringstream message;
        message << "Expected " << low << " <= " << name << " < " << high << "; got " << actual;
        throw std::domain_error(message.str());
    }
}

template<typename T>
inline void assert_in_range( const T high, const T actual, const char* name ) {
    assert_in_range(0, high, actual, name);
}

template<typename T>
inline void assert_point_within( const T x, const T y, const T width, const T height ){
    assert_in_range( width, x, "x");
    assert_in_range( height, y, "y");
}

/***************************************
 * 2D Board
 **************************************/
struct Board {
	int width = 0;
	int height = 0;
	std::set<Point> walls;

	Board( int aWidth, int aHeight )
	    : width(aWidth), height( aHeight )
	    {}

    void set_wall( int x, int y ){
        assert_point_within( x, y, width, height );

        walls.insert(std::make_pair(x,y));
    }

	bool has_wall_at( const Point& where ) const {
	    assert_point_within( where.first, where.second, width, height );
	    return walls.find(where) != walls.end();
	}

	bool has_wall_at( const int x, const int y ) const {
	    return has_wall_at(Point(x,y));
	}

	bool is_within( const Point &p) const {
		int x = p.first;
		int y = p.second;

		return (0 <= x && x < width)
			&& (0 <= y && y < height);
	}
};

inline std::ostream& operator<<( std::ostream& out, Board& board ){
    for( auto row = 0; row < board.height; row++ ){
        for( auto column = 0; column < board.width; column++){
            if( board.has_wall_at(column, row)){
                out << "#";
            }else{
                out << ".";
            }
        }
        out << std::endl;
    }
    return out;
}

template<typename DimensionType = size_t>
inline DimensionType horizontal_first_dimension_point( DimensionType x, DimensionType y, DimensionType width, DimensionType height ){
    return x + ( y * width );
}

struct BitBoard {
    int width, height;
    std::vector<bool> points;

    BitBoard( int aWidth, int aHeight )
        : width( aWidth ), height( aHeight ), points( aWidth * aHeight, 0)
        {}

    void assert_within( const Point where) const {
        assert_within(where.first, where.second);
    }

    void assert_within( const int x, const int y ) const {
	    assert_in_range( width, x, "x");
	    assert_in_range( height, y, "y");
    }

    void set_wall( int x, int y ){
        assert_within( x, y );

        points[horizontal_first_dimension_point(x,y, width, height)] = 1;
    }

    void set_wall( const Point& where ) {
        set_wall(where.first, where.second);
    }

	bool has_wall_at( const Point& where ) const {
	    return has_wall_at(where.first, where.second);
	}

	bool has_wall_at( const int x, const int y ) const {
	    assert_within(x,y);
	    return points[horizontal_first_dimension_point(x,y, width, height)];
	}

	bool is_within( const Point& p) const {
		int x = p.first;
		int y = p.second;

		return (0 <= x && x < width)
			&& (0 <= y && y < height);
	}
};

} //::pathfinder

#endif /* _pathfinder_board_h_ */