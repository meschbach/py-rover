#include<gtest/gtest.h>

#include "board.hpp"

using namespace rover;


TEST( PointLessThanComparator, less_than ){
    PointLessThanComparator ltc(1 << 24);
	ASSERT_TRUE( ltc( Point(1,1), Point(2,2) ) );
}

TEST( PointLessThanComparator, equal ){
    PointLessThanComparator ltc(1 << 24);
	ASSERT_FALSE( ltc( Point(42,42), Point(42,42) ) );
}

TEST( PointLessThanComparator, greater_than ){
    PointLessThanComparator ltc(1 << 12);
	ASSERT_FALSE( ltc( Point(2048,2048), Point(12,12) ) );
}
