'''Finds the closest pair of points in a 2-d plane.
For reference: http://en.wikipedia.org/wiki/Closest_pair_of_points_problem
'''

from collections import namedtuple
from itertools import combinations
import math


Point = namedtuple('Point', 'x y')


def distance(p1, p2):
    '''Returns the Euclidean distance between 2-d points `p1` and `p2`.'''
    return math.sqrt((p2.x - p1.x)**2 + (p2.y - p1.y)**2)


def find_closest_pair(points):
    '''Given a set of 2-d `points`, returns the pair of points that are closest
    in distance.
    '''
    p_x = sorted(points, key=lambda p: p.x)
    p_y = sorted(points, key=lambda p: p.y)
    return find_closet_pair_rec(p_x, p_y)


def find_closet_pair_rec(p_x, p_y):
    '''Given a set of 2-d points sorted by x-value (`p_x`) and y-value (`p_y`),
    returns the pair of points that are closest in distance.
    '''
    if len(p_x) < 4 or len(p_y) < 4:
        return min((pair for pair in combinations(set(p_x + p_y), 2)),
                   key=lambda pair: distance(*pair))

    middle = len(p_x) // 2
    closest_left_pair = find_closet_pair_rec(p_x[:middle], p_y[:middle])
    closest_right_pair = find_closet_pair_rec(p_x[middle:], p_y[middle:])

    closest_pair_candidates = [closest_left_pair, closest_right_pair]

    delta = min(distance(*pair) for pair in closest_pair_candidates)
    split_pair = find_closet_split_pair(p_x, p_y, delta)

    if split_pair is not None:
        closest_pair_candidates.append(split_pair)

    return min(closest_pair_candidates,
               key=lambda point_pair: distance(point_pair[0], point_pair[1]))


def find_closet_split_pair(p_x, p_y, delta):
    '''Given a set of 2-d points sorted by x-value (`p_x`) and y-value (`p_y`),
    and the previously calculated pairwise distance of the closest non-split
    pair of points (`delta`), returns the closest split pair of points with
    pairwise distance < `delta`, or None if no such pair exists.

    A split pair of points is a pair of points in which one point lies left of
    the vertical divide, x_mid, and the other point lies to the right.
    '''
    middle = len(p_x) // 2
    x_mid = p_x[middle].x

    # `s_y` is the subset of points that lie at most `delta` to the left or
    # right of the vertical divide, x_mid, sorted by y-value.
    s_y = []
    for p in p_y:
        if p.x >= x_mid - delta and p.x <= x_mid + delta:
            s_y.append(p)

    closest_split_pair = None
    min_delta = delta

    for i, p in enumerate(s_y):
        # We need only consider at most 6 neighboring points of `p`.
        for q in s_y[i + 1:i + 6]:
            d = distance(p, q)
            if d < delta:
                min_delta = d
                closest_split_pair = (p, q)

    return closest_split_pair


def find_closest_pair_bruteforce(points):
    return min((pair for pair in combinations(points, 2)),
               key=lambda pair: distance(*pair))


if __name__ == '__main__':
    points = [Point(x, y) for x, y in [(1, 0), (2, 6), (2, 9), (3, 1), (4, 9),
                                       (6, 0), (8, 6)]]
    assert(set(find_closest_pair(points)) == set((Point(2, 9), Point(4, 9))))
