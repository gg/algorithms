// Conway's Game of Life algorithm.
// For reference: http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>


struct Point {
    int x;
    int y;

    Point() {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return this->x == other.x && this->y == other.y;
    }
    
    bool operator<(const Point& other) const {
        if (this->x == other.x) {
            return this->y < other.y;
        }
        return this->x < other.x;
    }
};


void print_state(const std::set<Point>& state) {
    std::set<Point>::const_iterator it = state.begin();
    for (; it != state.end(); it++) {
        std::cout << "(" << it->x << ", " << it->y << "), ";
    }
    std::cout << std::endl;
}


std::set<Point> neighbors(const Point& p) {
    std::set<Point> neighboring_points;
    neighboring_points.insert(Point(p.x - 1, p.y - 1));
    neighboring_points.insert(Point(p.x - 1, p.y));
    neighboring_points.insert(Point(p.x - 1, p.y + 1));
    neighboring_points.insert(Point(p.x, p.y - 1));
    neighboring_points.insert(Point(p.x, p.y + 1));
    neighboring_points.insert(Point(p.x + 1, p.y - 1));
    neighboring_points.insert(Point(p.x + 1, p.y));
    neighboring_points.insert(Point(p.x + 1, p.y + 1));
    return neighboring_points;
}


std::set<Point> transition(const std::set<Point>& state) {
    std::set<Point> points_to_consider(state.begin(), state.end());
    std::set<Point>::iterator it;
    for (it = state.begin(); it != state.end(); it++) {
        std::set<Point> neighboring_points(neighbors(*it));
        std::copy(neighboring_points.begin(), neighboring_points.end(),
                  std::inserter(points_to_consider,
                                points_to_consider.begin()));
    }

    std::set<Point> next_state;

    for (it = points_to_consider.begin(); it != points_to_consider.end();
         it++) {

        std::set<Point> neighboring_points(neighbors(*it)); 

        int num_alive_neighbors = 0;
        for (std::set<Point>::iterator j = neighboring_points.begin();
            j != neighboring_points.end(); j++) {
            const bool alive = state.find(*j) != state.end();
            if (alive) {
                num_alive_neighbors++;
            }
        }

        const bool alive = state.find(*it) != state.end();
        if (num_alive_neighbors == 3 or (num_alive_neighbors == 2 and alive)) {
            next_state.insert(*it); 
        }
    }

    return next_state;
}


template <class RandomAccessIterator>
bool test_pattern(RandomAccessIterator first_state,
                  RandomAccessIterator last_state) {
    for (RandomAccessIterator i = first_state; i != last_state - 1; i++) {
        if (transition(*i) != *(i + 1)) {
            std::cout << "transition: " << i - first_state << " -> "
                << i - first_state + 1 << " failed" << std::endl;
            print_state(transition(*i));
            std::cout << std::endl;
            print_state(*(i + 1));
            return false;
        }
    }
    return true;
}


int main(int argc, char** argv) {
    typedef std::set<Point> State;

    // Still lifes
    { // Block
        std::vector<State> block;
        Point s1[] = {Point(1, 1), Point(1, 2), Point(2, 1), Point(2, 2)};
        block.push_back(State(s1, s1 + 4));
        block.push_back(block[0]);
        assert(test_pattern(block.begin(), block.end()));
    }
    { // Behive
        std::vector<State> beehive;
        Point s1[] = {Point(1, 2), Point(1, 3), Point(2, 1), Point(2, 4),
                      Point(3, 2), Point(3, 3)};
        beehive.push_back(State(s1, s1 + 6));
        beehive.push_back(beehive[0]);
        assert(test_pattern(beehive.begin(), beehive.end()));
    }
    { // Loaf
        std::vector<State> loaf;
        Point s1[] = {Point(1, 2), Point(1, 3), Point(2, 1), Point(2, 4),
                      Point(3, 2), Point(3, 4), Point(4, 3)};
        loaf.push_back(State(s1, s1 + 7));
        loaf.push_back(loaf[0]);
        assert(test_pattern(loaf.begin(), loaf.end()));
    }
    { // Boat
        std::vector<State> boat;
        Point s1[] = {Point(1, 1), Point(1, 2), Point(2, 1), Point(2, 3),
                      Point(3, 2)};
        boat.push_back(State(s1, s1 + 5));
        boat.push_back(boat[0]);
        assert(test_pattern(boat.begin(), boat.end()));
    }
       
 
    // Oscillators
    { // Blinker
        std::vector<State> blinker;
        Point s1[] = {Point(1, 0), Point(1, 1), Point(1, 2)};
        Point s2[] = {Point(2, 1), Point(1, 1), Point(0, 1)};
        blinker.push_back(State(s1, s1 + 3));
        blinker.push_back(State(s2, s2 + 3));
        blinker.push_back(blinker[0]);
        assert(test_pattern(blinker.begin(), blinker.end()));
    }
    { // Toad
        std::vector<State> toad;
        Point s1[] = {Point(2, 2), Point(2, 3), Point(2, 4), Point(3, 1),
                      Point(3, 2), Point(3, 3)};
        Point s2[] = {Point(1, 3), Point(2, 1), Point(2, 4), Point(3, 1),
                      Point(3, 4), Point(4, 2)};
        toad.push_back(State(s1, s1 + 6));
        toad.push_back(State(s2, s2 + 6));
        toad.push_back(toad[0]);
        assert(test_pattern(toad.begin(), toad.end()));
    }
    { // Beacon
        std::vector<State> beacon;
        Point s1[] = {Point(1, 1), Point(1, 2), Point(2, 1), Point(2, 2),
                      Point(3, 3), Point(3, 4), Point(4, 3), Point(4, 4)};
        Point s2[] = {Point(1, 1), Point(1, 2), Point(2, 1), Point(3, 4),
                      Point(4, 3), Point(4, 4)};
        beacon.push_back(State(s1, s1 + 8));
        beacon.push_back(State(s2, s2 + 6));
        beacon.push_back(beacon[0]);
        assert(test_pattern(beacon.begin(), beacon.end()));
    }
    { // Pulsar
        std::vector<State> pulsar;

        Point s1[] = {Point(1, 5), Point(1, 11),
                      Point(2, 5), Point(2, 11),
                      Point(3, 5), Point(3, 6), Point(3, 10), Point(3, 11),
                      Point(5, 1), Point(5, 2), Point(5, 3), Point(5, 6),
                      Point(5, 7), Point(5, 9), Point(5, 10), Point(5, 13),
                      Point(5, 14), Point(5, 15),
                      Point(6, 3), Point(6, 5), Point(6, 7), Point(6, 9),
                      Point(6, 11), Point(6, 13),
                      Point(7, 5), Point(7, 6), Point(7, 10), Point(7, 11),
                      Point(9, 5), Point(9, 6), Point(9, 10), Point(9, 11),
                      Point(10, 3), Point(10, 5), Point(10, 7), Point(10, 9),
                      Point(10, 11), Point(10, 13),
                      Point(11, 1), Point(11, 2), Point(11, 3), Point(11, 6),
                      Point(11, 7), Point(11, 9), Point(11, 10), Point(11, 13),
                      Point(11, 14), Point(11, 15),
                      Point(13, 5), Point(13, 6), Point(13, 10), Point(13, 11),
                      Point(14, 5), Point(14, 11),
                      Point(15, 5), Point(15, 11)};

        Point s2[] = {Point(2, 4), Point(2, 5), Point(2, 11), Point(2, 12),
                      Point(3, 5), Point(3, 6), Point(3, 10), Point(3, 11),
                      Point(4, 2), Point(4, 5), Point(4, 7), Point(4, 9),
                      Point(4, 11), Point(4, 14),
                      Point(5, 2), Point(5, 3), Point(5, 4), Point(5, 6),
                      Point(5, 7), Point(5, 9), Point(5, 10), Point(5, 12),
                      Point(5, 13), Point(5, 14),
                      Point(6, 3), Point(6, 5), Point(6, 7), Point(6, 9),
                      Point(6, 11), Point(6, 13),
                      Point(7, 4), Point(7, 5), Point(7, 6), Point(7, 10),
                      Point(7, 11), Point(7, 12),
                      Point(9, 4), Point(9, 5), Point(9, 6), Point(9, 10),
                      Point(9, 11), Point(9, 12),
                      Point(10, 3), Point(10, 5), Point(10, 7), Point(10, 9),
                      Point(10, 11), Point(10, 13),
                      Point(11, 2), Point(11, 3), Point(11, 4), Point(11, 6),
                      Point(11, 7), Point(11, 9), Point(11, 10), Point(11, 12),
                      Point(11, 13), Point(11, 14),
                      Point(12, 2), Point(12, 5), Point(12, 7), Point(12, 9),
                      Point(12, 11), Point(12, 14),
                      Point(13, 5), Point(13, 6), Point(13, 10), Point(13, 11),
                      Point(14, 4), Point(14, 5), Point(14, 11),
                      Point(14, 12)};

        Point s3[] = {Point(2, 4), Point(2, 5), Point(2, 6), Point(2, 10),
                      Point(2, 11), Point(2, 12),
                      Point(4, 2), Point(4, 7), Point(4, 9), Point(4, 14),
                      Point(5, 2), Point(5, 7), Point(5, 9), Point(5, 14),
                      Point(6, 2), Point(6, 7), Point(6, 9), Point(6, 14),
                      Point(7, 4), Point(7, 5), Point(7, 6), Point(7, 10),
                      Point(7, 11), Point(7, 12),
                      Point(9, 4), Point(9, 5), Point(9, 6), Point(9, 10),
                      Point(9, 11), Point(9, 12),
                      Point(10, 2), Point(10, 7), Point(10, 9), Point(10, 14),
                      Point(11, 2), Point(11, 7), Point(11, 9), Point(11, 14),
                      Point(12, 2), Point(12, 7), Point(12, 9), Point(12, 14),
                      Point(14, 4), Point(14, 5), Point(14, 6), Point(14, 10),
                      Point(14, 11), Point(14, 12)};

        pulsar.push_back(State(s1, s1 + (sizeof s1 / sizeof s1[0])));
        pulsar.push_back(State(s2, s2 + (sizeof s2 / sizeof s2[0])));
        pulsar.push_back(State(s3, s3 + (sizeof s3 / sizeof s3[0])));
        pulsar.push_back(pulsar[0]);
        assert(test_pattern(pulsar.begin(), pulsar.end()));
    }


    // Spaceships
    { // Glider
        std::vector<State> glider;
        Point s1[] = {Point(1, 1), Point(2, 2), Point(2, 3), Point(3, 1),
                      Point(3, 2)};
        Point s2[] = {Point(1, 2), Point(2, 3), Point(3, 1), Point(3, 2),
                      Point(3, 3)};
        Point s3[] = {Point(2, 1), Point(2, 3), Point(3, 2), Point(3, 3),
                      Point(4, 2)};
        Point s4[] = {Point(2, 3), Point(3, 1), Point(3, 3), Point(4, 2),
                      Point(4, 3)};
        Point s5[] = {Point(2, 2), Point(3, 3), Point(3, 4), Point(4, 2),
                      Point(4, 3)};
        glider.push_back(State(s1, s1 + 5));
        glider.push_back(State(s2, s2 + 5));
        glider.push_back(State(s3, s3 + 5));
        glider.push_back(State(s4, s4 + 5));
        glider.push_back(State(s5, s5 + 5));
        assert(test_pattern(glider.begin(), glider.end()));
    }

    std::cout << "Tests passed." << std::endl;
}
