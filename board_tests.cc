#include<gtest/gtest.h>

#include "board.hpp"

using namespace rover;

TEST( Board, top_right ){
	Board board = { 2, 2 };
	ASSERT_TRUE( board.is_within( Point(1,0) ) );
}

TEST( Board, top_left ){
	Board board = { 2, 2 };
	ASSERT_TRUE( board.is_within( Point(0,0) ) );
}

TEST( Board, bottom_right ){
	Board board = { 2, 2 };
	ASSERT_TRUE( board.is_within( Point(1,1) ) );
}

TEST( Board, bottom_left ){
	Board board = { 2, 2 };
	ASSERT_TRUE( board.is_within( Point(0,1) ) );
}

TEST( Board_walls, default_doesnt_have_walls ){
    Board board = { 128, 128 };
    ASSERT_FALSE(board.has_wall_at(42,12));
}

TEST( Board_walls, does_have_wall ){
    Board board = { 16, 16 };
    board.set_wall( 15, 15);
    ASSERT_TRUE(board.has_wall_at(15,15));
}

TEST( Board_within, not_diagonial_above_left){
    Board board = { 7, 7 };
    ASSERT_FALSE(board.is_within(Point(-1,-1)));
}

TEST( Board_within, not_diagonial_below_right){
    Board board = { 7, 7 };
    ASSERT_FALSE(board.is_within(Point(7,7)));
}