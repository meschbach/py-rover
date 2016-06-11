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
    if( actual < low || high <= actual) {
        std::stringstream message;
        message << "Expected " << low << " <= " << name << " < " << high << "; got " << actual;
        throw std::domain_error(message.str());
    }
}

template<typename T>
inline void assert_in_range( const T high, const T actual, const char* name ) {
    assert_in_range(0, high, actual, name);
}

/***************************************
 * 2D Board
 **************************************/
struct Board {
	int width = 0;
	int height = 0;
	std::set<Point> walls;

    void set_wall( int x, int y ){
        assert_in_range( width, x, "x");
        assert_in_range( height, y, "y");

        walls.insert(std::make_pair(x,y));
    }

	bool has_wall_at( const Point& where ) const {
	    assert_in_range( width, where.first, "x");
	    assert_in_range( height, where.second, "y");

	    return walls.find(where) != walls.end();
	}

	bool has_wall_at( const int x, const int y ) const {
	    return has_wall_at(Point(x,y));
	}

	bool is_within( const Point p) const {
		int x = p.first;
		int y = p.second;

		return 0 <= x && x < width
			&& 0 <= y && y < height;
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

} //::pathfinder

#endif /* _pathfinder_board_h_ */