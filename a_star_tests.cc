#include<gtest/gtest.h>

//
#include<map>
#include<iterator>

#include "a_star.hpp"
using namespace std;

TEST(a_star, adjacent_up){
	Board board = { 2, 2 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(0,1), Point(0,0) );

	vector<Point> expected;
	expected.push_back( Point(0,0 ) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, adjacent_down){
	Board board = { 2, 2 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(0,0), Point(0,1) );

	vector<Point> expected;
	expected.push_back( Point(0,1 ) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, adjacent_right){
	Board board = { 2, 2 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(0,1), Point(1,1) );

	vector<Point> expected;
	expected.push_back( Point(1,1 ) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, adjacent_left ){
	Board board = { 2, 2 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(1,1), Point(0,1) );

	vector<Point> expected;
	expected.push_back( Point(0,1 ) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, diagonal_upper_right ){
	Board board = { 3, 3 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(1,1), Point(2,0) );

	vector<Point> expected;
	expected.push_back( Point(2,0) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, diagonal_lower_left ){
	Board board = { 3, 3 };

	AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(1,1), Point(0,2) );

	vector<Point> expected;
	expected.push_back( Point(0,2) );
	EXPECT_EQ( expected, points );
}

TEST(a_star, large_simple_path) {
    Board board = {8,8};
	vector<Point> expected;
	for( long moves = 0; moves <= 7; moves++){
	    expected.push_back(make_pair(moves,moves));
	}

    AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(8,8), Point(0,0) );
	EXPECT_EQ( expected, points );
}


TEST(a_star, navigates_obsticle) {
    Board board = {8,8};
    for( auto i = 1 ; i < 7; i++ ) board.set_wall( 4, i);
    for( auto i = 0 ; i < 4; i++ ) board.set_wall( i, 7);
    for( auto i = 2 ; i < 7; i++ ) board.set_wall( i, 1);

	vector<Point> expected = {
	    make_pair(5, 4),
	    make_pair(5, 3),
	    make_pair(6, 2),
	    make_pair(7, 1),
	    make_pair(6, 0),
	    make_pair(5, 0),
	    make_pair(4, 0),
	    make_pair(3, 0),
	    make_pair(2, 0),
	    make_pair(1, 1),
	    make_pair(0, 2),
	    make_pair(1, 3),
	    make_pair(0, 4),
	    make_pair(1, 5) };

    AStar pathfinder;
	vector<Point> points = pathfinder.findFor( board, Point(0,6), Point(5,4) );
	EXPECT_EQ( expected, points );
}
