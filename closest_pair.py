'''Algorithm to find the closest pair of points in a 2-d plane.
For reference: http://en.wikipedia.org/wiki/Closest_pair_of_points_problem
'''

from collections import namedtuple
from itertools import combinations
import math


Point = namedtuple('Point', 'x y')


def distance(p1, p2):
    '''Returns the Euclidean distance between 2-d points `p1` and `p2`.'''
    return math.sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2)


def find_closest_pair_bruteforce(points):
    '''Given a collection of unique `points`, returns the closest pair of
    points using an O(n^2) brute-force approach that compares pairwise
    distances for all pairs of points.'''
    return min((pair for pair in combinations(points, 2)),
               key=lambda pair: distance(*pair))


def find_closet_split_pair(p_x, p_y, delta):
    '''Given a collection of unique points sorted by x-value and y-value, `p_x`
    and `p_y` respectively, and the previously calculated pairwise distance
    between the closest non-split pair of points, `delta`, returns the closest
    split pair of points with pairwise distance < `delta`. Returns None if no
    split pair is found with pairwise distance less than `delta`.

    A split pair of points is a pair of points in which one point lies left of
    the vertical divide x_mid, the midpoint of `p_x`, and the other point lies
    to the right.
    '''
    middle = len(p_x) // 2
    x_mid = p_x[middle].x

    # `s_y` is the subset of points that lie at most `delta` to the left or
    # right of the vertical divide x_mid, sorted by y-value.
    s_y = [p for p in p_y if abs(p.x - x_mid) <= delta]

    closest_split_pair = None
    min_delta = delta

    # For each point in sy, we need only consider at most 6 of its neighbors.
    for i, p in enumerate(s_y):
        for q in s_y[i + 1:i + 6]:
            d = distance(p, q)
            if d < delta:
                min_delta = d
                closest_split_pair = (p, q)

    return closest_split_pair


def find_closet_pair_rec(p_x, p_y):
    '''Given a collection of unique points sorted by x-value (`p_x`) and
    y-value (`p_y`), returns the pair of points that are closest in distance
    using an O(nlogn) divide-and-conquer algorithm.
    '''
    if len(p_x) < 4 or len(p_y) < 4:
        return find_closest_pair_bruteforce(set(p_x + p_y))

    middle = len(p_x) // 2
    closest_left_pair = find_closet_pair_rec(p_x[:middle], p_y[:middle])
    closest_right_pair = find_closet_pair_rec(p_x[middle:], p_y[middle:])

    closest_pair_candidates = [closest_left_pair, closest_right_pair]

    delta = min(distance(*pair) for pair in closest_pair_candidates)
    closest_split_pair = find_closet_split_pair(p_x, p_y, delta)

    if closest_split_pair is not None:
        closest_pair_candidates.append(closest_split_pair)

    return min(closest_pair_candidates,
               key=lambda point_pair: distance(*point_pair))


def find_closest_pair(points):
    '''Given a collection of unique `points`, returns the pair of points that
    are closest in distance using an O(nlogn) divide-and-conquer algorithm.
    '''
    p_x = sorted(points, key=lambda p: p.x)
    p_y = sorted(points, key=lambda p: p.y)
    return find_closet_pair_rec(p_x, p_y)


if __name__ == '__main__':
    points = [Point(x, y) for x, y in [(1, 0), (2, 6), (2, 9), (3, 1), (4, 9),
                                       (6, 0), (8, 6)]]
    assert(set(find_closest_pair(points)) == set((Point(2, 9), Point(4, 9))))
