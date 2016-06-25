/*
 *
 */
#include<algorithm>
#include<iostream>
#include<iomanip>
#include<map>
#include<set>
#include<stack>
#include<queue>
#include<vector>

#include "board.hpp"
#include "point.hpp"

namespace rover {

/***************************************
 *
 **************************************/
const std::vector<Point> neighbors_mask = { {0, 1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };

struct ScoredPoint {
    int score;
    Point where;

    ScoredPoint( int aScore, Point aPlace ) : score(aScore), where(aPlace) {}
};

inline std::ostream& operator<<( std::ostream& out, const ScoredPoint scored ){
    return out << "[" << scored.where << " @ " << scored.score <<" ]";
}

inline bool operator<( const ScoredPoint &lhs, const ScoredPoint &rhs ){
    auto result = lhs.score < rhs.score;
    return result;
}

inline bool operator>( const ScoredPoint &lhs, const ScoredPoint &rhs ){
    auto result = lhs.score > rhs.score;
    return result;
}


template<typename Terrain>
struct AStarContext {
    BitBoard close_set;
    std::set<Point, PointLessThanComparator> outstanding_index;
    std::priority_queue< ScoredPoint, std::vector<ScoredPoint>, std::greater<ScoredPoint> > outstanding_points;
    std::map<Point,Point, PointLessThanComparator> cameFrom;

    AStarContext(const Terrain &board)
        : close_set(board.width, board.height)
        {}

    void outstanding_point( Point where, int guessed_score)  {
        auto outstanding = ScoredPoint(guessed_score, where);
        outstanding_points.push(outstanding);
        outstanding_index.insert(where);
    }

    bool has_outstanding_points() const {
        return !outstanding_points.empty();
    }

    Point pop_oustanding() {
        auto score_point_pair = outstanding_points.top();
        auto point = score_point_pair.where;

        outstanding_points.pop();
        outstanding_index.erase(point);
        return point;
    }

    bool point_outstanding(Point where) const {
        return outstanding_index.find(where) != outstanding_index.end();
    }

    void mark_closed( Point where ){
        close_set.set_wall( where );
    }

    bool is_closed( Point where) const {
        return close_set.has_wall_at(where);
    }

    void came_from( Point from, Point to ) {
        cameFrom[to] = from;
    }
};

template<typename Terrain>
struct AStar {
    using ScoredPoints = std::map< Point, int, PointLessThanComparator >;
    using PointAssociations = std::map< Point, Point, PointLessThanComparator >;

	int heuristic( const Point& a, const Point& b ) const {
		int first = b.first - a.first;
		int second = b.second - a.second;
		return (first*first) + (second*second);  //Taxi cab distance, squared
	}

	std::vector<Point> build_path(const PointAssociations& cameFrom, Point current) const {
        std::vector<Point> data;
        size_t cap = 16777216;
        while( cameFrom.find( current ) != cameFrom.end() && cap != 0 ) {
            data.push_back( current );
            //TODO: This does a lookup twice
            auto pair = cameFrom.find(current);
            current = pair->second;

            cap--;
        }
        return data;
	}

	std::vector<Point> findFor( Terrain &board, const Point start, const Point goal ) {
	    unsigned int timeToLive = 1 << 30;
	    if( !board.is_within(start) || board.has_wall_at(start)) {
	        return std::vector<Point>();
	    }
	    if( !board.is_within(goal) || board.has_wall_at(goal)){
	        return std::vector<Point>();
	    }

	    if( goal == start ){
	        return std::vector<Point>();
	    }

	    AStarContext<Terrain> context(board);
		const std::vector<Point> neighbors = neighbors_mask;
		ScoredPoints actual_score;
		actual_score[ start ] = 0;

		ScoredPoints guessed_score;
		guessed_score[ start ] = heuristic( start, goal );

        context.outstanding_point( start, guessed_score[start]);

		while( context.has_outstanding_points()){
            if( timeToLive == 0 ){
                return std::vector<Point>();
            }else{
                timeToLive--;
            }

			auto current = context.pop_oustanding();
			if( current == goal ){
                return build_path( context.cameFrom, current );
			}
			if( board.has_wall_at(current)){
			    continue;
			}

            context.mark_closed(current);

			for( auto neighbors_mask = neighbors.begin(); neighbors_mask != neighbors.end(); neighbors_mask++ ){
				auto mask = *neighbors_mask;
				auto neighbor = current + mask;

 				if( !board.is_within( neighbor ) ) {
				    continue;
                }
                if( board.has_wall_at( neighbor ) ){
                    continue;
                }

				auto tentative_g_score = actual_score[current] + heuristic(current, neighbor);
				if( context.is_closed(neighbor) ){
					auto current_score = actual_score[neighbor];
					if( tentative_g_score >= current_score ){
						continue;
					}
				}

				if( neighbor_has_lower_score( tentative_g_score, neighbor,  actual_score ) || !context.point_outstanding( neighbor ) ){
				    found_better_path(context, actual_score, guessed_score, current, neighbor, goal, tentative_g_score);
				}
			}
		}
		return std::vector<Point>();
	}

	void found_better_path(AStarContext<Terrain> &context,
	        ScoredPoints &actual_score,
	        ScoredPoints &guessed_score,
	        const Point current,
	        const Point neighbor,
	        const Point goal,
	        int tentative_g_score){
        context.came_from( current, neighbor);
        actual_score[ neighbor ] = tentative_g_score;
        guessed_score[ neighbor ] = tentative_g_score + heuristic( neighbor, goal );
        context.outstanding_point(neighbor, guessed_score[neighbor]);
	}

	bool neighbor_has_lower_score( const int score, const Point& neighbor, ScoredPoints &actual_score ) {
		if( actual_score.find( neighbor ) == actual_score.end() ){
			return true;
		}
		int previous_score = actual_score[ neighbor ];
		bool replace = score < previous_score;
		return replace;
	}
};

}