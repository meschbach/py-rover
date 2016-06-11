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
std::vector<Point> neighbors_mask = { {0, 1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };

struct AStarContext {
    std::set<Point> close_set;
    std::set<Point> outstanding_index;
    std::priority_queue< std::pair<int,Point> > outstanding_points;
    std::map<Point,Point> cameFrom;

    void outstanding_point( Point where, int guessed_score)  {
        auto outstanding = make_pair(guessed_score, where);
        outstanding_points.push(outstanding);
        outstanding_index.insert(where);
    }

    bool has_outstanding_points() const {
        return !outstanding_points.empty();
    }

    Point pop_oustanding() {
        auto score_point_pair = outstanding_points.top();
        auto point = score_point_pair.second;
        outstanding_points.pop();
        outstanding_index.erase(point);
        return point;
    }

    bool point_outstanding(Point where) const {
        return outstanding_index.find(where) != outstanding_index.end();
    }

    void mark_closed( Point where ){
        close_set.insert(where);
    }

    bool is_closed( Point where) const {
        return close_set.find(where) != close_set.end();
    }

    void came_from( Point from, Point to ) {
        cameFrom[to] = from;
    }
};

struct AStar {
	int heuristic( Point a, Point b ){
		int first = b.first - a.first;
		int second = b.second - a.second;
		return (first*first) + (second*second);  //Taxi cab distance, squared
	}

	std::vector<Point> build_path(const std::map<Point,Point>& cameFrom, Point current) const {
        std::vector<Point> data;
        while( cameFrom.find( current ) != cameFrom.end() ) {
            data.push_back( current );
            //TODO: This does a lookup twice
            auto pair = (*cameFrom.find(current));
            current = pair.second;
        }
        return data;
	}

	std::vector<Point> findFor( Board &board, const Point start, const Point goal ) {
	    unsigned int timeToLive = 1 << 31;
	    if( board.has_wall_at(start)) {
//	        std::cout << "Wall at starting point " << start << std::endl;
	        return std::vector<Point>();
	    }
	    if( board.has_wall_at(goal)){
//	        std::cout << "Wall at starting point " << goal << std::endl;
	        return std::vector<Point>();
	    }

	    if( goal == start ){
	        return std::vector<Point>();
	    }

	    AStarContext context;
		std::vector<Point> neighbors = neighbors_mask;
		std::map<Point,int> actual_score;
		actual_score[ start ] = 0;

		std::map<Point,int> guessed_score;
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
				Point mask = *neighbors_mask;
				Point neighbor = current + mask;

				if( !board.is_within( neighbor ) || board.has_wall_at( neighbor ) ) {
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
				    context.came_from( current, neighbor);
					actual_score[ neighbor ] = tentative_g_score;
					guessed_score[ neighbor ] = tentative_g_score + heuristic( neighbor, goal );
					context.outstanding_point(neighbor, guessed_score[neighbor]);
				}
			}
		}
		return std::vector<Point>();
	}

	bool neighbor_has_lower_score( int score, Point neighbor, std::map<Point,int> &actual_score ) {
		if( actual_score.find( neighbor ) == actual_score.end() ){
			return true;
		}
		int previous_score = actual_score[ neighbor ];
		bool replace = score < previous_score;
		return replace;
	}
};

}