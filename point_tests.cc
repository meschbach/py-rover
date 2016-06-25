#include<gtest/gtest.h>

#include "board.hpp"

using namespace rover;


TEST( BoundedPointLessThanComparator, less_than ){
    BoundedPointLessThanComparator ltc(1 << 24);
	ASSERT_TRUE( ltc( Point(1,1), Point(2,2) ) );
}

TEST( BoundedPointLessThanComparator, equal ){
    BoundedPointLessThanComparator ltc(1 << 24);
	ASSERT_FALSE( ltc( Point(42,42), Point(42,42) ) );
}

TEST( BoundedPointLessThanComparator, greater_than ){
    BoundedPointLessThanComparator ltc(1 << 12);
	ASSERT_FALSE( ltc( Point(2048,2048), Point(12,12) ) );
}
