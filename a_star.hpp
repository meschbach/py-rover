/*
 *
 */
#include<iomanip>
#include<map>
#include<stack>
#include<vector>
#include<algorithm>

#include "board.hpp"

//TODO: Scope this properly
using namespace std;
using namespace pathfinder;

/***************************************
 *
 **************************************/
vector<Point> neighbors_mask = { {0, 1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };

struct AStarContext {
    vector<Point> close_set;
    vector< pair<int,Point> > outstanding_points;
    map<Point,Point> cameFrom;

    void outstanding_point( Point where, int guessed_score)  {
        auto outstanding = make_pair(guessed_score, where);
        outstanding_points.push_back(outstanding);
    }

    bool has_outstanding_points() const {
        return !outstanding_points.empty();
    }

    Point pop_oustanding() {
        auto score_point_pair = outstanding_points.back();
        auto point = score_point_pair.second;
        outstanding_points.pop_back();
        return point;
    }

    bool point_outstanding(Point where) const {
		for( auto it = outstanding_points.begin(); it != outstanding_points.end(); it++ ){
			auto pair = *it;
			if( pair.second == where ){
				return true;
			}
		}
		return false;
    }

    void mark_closed( Point where ){
        close_set.push_back(where);
    }

    bool is_closed( Point where) const {
        return find( close_set.begin(), close_set.end(), where ) != close_set.end();
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

	std::vector<Point> build_path(const map<Point,Point>& cameFrom, Point current) const {
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
	    AStarContext context;
		vector<Point> neighbors = neighbors_mask;
		map<Point,int> actual_score;
		actual_score[ start ] = 0;

		map<Point,int> guessed_score;
		guessed_score[ start ] = heuristic( start, goal );

        context.outstanding_point( start, guessed_score[start]);

		while( context.has_outstanding_points()){
			auto current = context.pop_oustanding();

			if( current == goal ){
			    cout << "Found goal" << endl;
                return build_path( context.cameFrom, current );
			}

            context.mark_closed(current);

			for( auto neighbors_mask = neighbors.begin(); neighbors_mask != neighbors.end(); neighbors_mask++ ){
				Point mask = *neighbors_mask;
				Point neighbor = current + mask;

                cout << "???\tNeighbor " << neighbor << endl;
				if( !board.is_within( neighbor ) || board.has_wall_at( neighbor ) ) {
				    cout << "---\tRejected beacuse wall or out of bounds" << endl;
				    continue;
                }

				auto tentative_g_score = actual_score[current] + heuristic(current, neighbor);
				if( context.is_closed(neighbor) ){
					auto current_score = actual_score[neighbor];
					if( tentative_g_score >= current_score ){
					    cout << "---\tRejected because worse score and closed" << endl;
						continue;
					}
				}

				if( neighbor_has_lower_score( tentative_g_score, neighbor,  actual_score ) || !context.point_outstanding( neighbor ) ){
				    cout << "+++\tAdding " << neighbor << endl;
				    context.came_from( current, neighbor);
					actual_score[ neighbor ] = tentative_g_score;
					guessed_score[ neighbor ] = tentative_g_score + heuristic( neighbor, goal );
					context.outstanding_point(neighbor, guessed_score[neighbor]);
				}
			}
		}
		return std::vector<Point>();
	}

	bool neighbor_has_lower_score( int score, Point neighbor, map<Point,int> actual_score ) {
		if( actual_score.find( neighbor ) == actual_score.end() ){
			return true;
		}
		int previous_score = actual_score[ neighbor ];
		bool replace = score < previous_score;
		return replace;
	}
};
