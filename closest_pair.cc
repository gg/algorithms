// Copyright (c) 2012 Gregg Gajic <gregg.gajic@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

// Closest pair of points
// See: http://en.wikipedia.org/wiki/Closest_pair_of_points_problem

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cfloat>
#include <iterator>
#include <set>
#include <utility>
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


typedef std::pair<Point, Point> PointPair;


// Returns the Euclidean distance between 2-d points p1 and p2.
float distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}


struct sort_by_x_value {
    bool operator()(const Point& p1, const Point& p2) const {
        return p1.x < p2.x;
    }
};


struct sort_by_y_value {
    bool operator()(const Point& p1, const Point& p2) const {
        return p1.y < p2.y;
    }
};


// Comparison functor used to determine if pair1 is closer in distance than
// pair2, for use with std::min as follows.
struct is_closer_in_distance {
    bool operator()(const PointPair& pair1, const PointPair& pair2) const {
        float pair1_distance = distance(pair1.first, pair1.second);
        float pair2_distance = distance(pair2.first, pair2.second);
        return pair1_distance < pair2_distance;
    }
};


// Given a pair of iterators to a collection of unique points, returns the
// closest pair of points in range [first, last) using an O(n^2) brute-force
// approach that compares pairwise distances for all pairs of points in [first,
// last).
template <class InputIterator>
PointPair find_closest_pair_bruteforce(InputIterator first,
                                       InputIterator last) {
    float min_distance = FLT_MAX;
    PointPair closest_pair;

    InputIterator i, j;
    for (i = first; i != last; i++) {
        j = i;
        j++;
        for (; j != last; j++) {
            float dist = distance(*i, *j);
            if (dist < min_distance) {
                min_distance = dist;
                closest_pair.first = *i;
                closest_pair.second = *j;
            }
        }
    }

    return closest_pair;
}


// Given a pair of iterators to a collection of unique points sorted by x-value
// and y-value, Px [px_begin, px_end) and Py [py_begin, py_end) respectively,
// and the previously calculated closest_non_split_pair of points, with
// pairwise distance delta, returns the closest split pair of points with
// pairwise distance less than delta. Returns closest_non_split_pair if no
// split pair is found with pairwise distance less than delta.
//
// A split pair of points is a pair of points in which one point lies left of
// the vertical divide x_mid, the midpoint of Px, and the other point lies to
// the right.
template <class RandomAccessIterator>
PointPair find_closest_split_pair(RandomAccessIterator px_begin,
                                  RandomAccessIterator px_end,
                                  RandomAccessIterator py_begin,
                                  RandomAccessIterator py_end,
                                  const PointPair& closest_non_split_pair) {
    Point px_mid = *(px_begin + ((px_end - px_begin) / 2));

    float delta = distance(closest_non_split_pair.first,
                           closest_non_split_pair.second);

    // sy is the subset of points that lie at most delta to the left or right
    // of the vertical divide x_mid, sorted by y-value.
    std::vector<Point> sy;

    RandomAccessIterator py_iter = py_begin;
    for (; py_iter != py_end; py_iter++) {
        if (std::abs(py_iter->x - px_mid.x) <= delta) {
            sy.push_back(*py_iter);
        }
    }

    PointPair closest_split_pair = closest_non_split_pair;
    float min_delta = delta;


    // For each point in sy, we need only consider at most 6 of its neighbors.
    std::vector<Point>::iterator i, j;
    for (i = sy.begin(); i != sy.end(); i++) {
        for (j = i + 1; j != i + 7 && j != sy.end(); j++) {
            float delta = distance(*i, *j);
            if (delta < min_delta) {
                min_delta = delta;
                closest_split_pair.first = *i;
                closest_split_pair.second = *j;
            }
        }
    }

    return closest_split_pair;
}


// Given a pair of iterators to a collection of unique points sorted by x-value
// and y-value, Px [px_begin, px_end) and Py [py_begin, py_end) respectively,
// returns the pair of points that are closest in distance using an O(nlogn)
// divide-and-conquer algorithm.
template <class RandomAccessIterator>
PointPair find_closest_pair_rec(RandomAccessIterator px_begin,
                                RandomAccessIterator px_end,
                                RandomAccessIterator py_begin,
                                RandomAccessIterator py_end) {
    if ((px_end - px_begin < 4) || (py_end - py_begin < 4)) {
        std::set<Point> points(px_begin, px_end);
        std::copy(py_begin, py_end, std::inserter(points, points.begin()));
        return find_closest_pair_bruteforce(points.begin(), points.end());
    }

    int middle = (px_end - px_begin) / 2;

    PointPair closest_pair_left = find_closest_pair_rec(px_begin,
                                                        px_begin + middle,
                                                        py_begin,
                                                        py_begin + middle);

    PointPair closest_pair_right = find_closest_pair_rec(px_begin + middle,
                                                         px_end, 
                                                         py_begin + middle,
                                                         py_end);

    PointPair closest_pair = std::min(closest_pair_left, closest_pair_right,
                                      is_closer_in_distance());

    PointPair closest_split_pair = find_closest_split_pair(px_begin, px_end,
                                                           py_begin, py_end,
                                                           closest_pair);

    return std::min(closest_pair, closest_split_pair, is_closer_in_distance());
}


// Given a pair of iterators to a collection of unique points, returns the pair
// of points that are closest in distance using an O(nlogn) divide-and-conquer
// algorithm.
template <class InputIterator>
PointPair find_closest_pair(InputIterator first, InputIterator last) {
    std::vector<Point> px(first, last);
    std::sort(px.begin(), px.end(), sort_by_x_value());

    std::vector<Point> py(first, last);
    std::sort(py.begin(), py.end(), sort_by_y_value());

    return find_closest_pair_rec(px.begin(), px.end(), py.begin(), py.end());
}


int main(int argc, char** argv) {
    Point tmp[] = {Point(1, 0), Point(2, 6), Point(2, 9), Point(3, 1), 
                   Point(4, 9), Point(6, 0), Point(8, 6)};

    std::vector<Point> points(tmp, tmp + (sizeof tmp / sizeof tmp[0]));

    PointPair expected(Point(2, 9), Point(4, 9));

    assert(find_closest_pair_bruteforce(points.begin(), points.end())
           == expected);
    assert(find_closest_pair(points.begin(), points.end()) == expected);
}
