# coding: utf-8

import itertools
import random


def shuffle(A):
    '''Shuffles `A` in-place using the Fischer-Yates algorithm.
    See: http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    '''
    for i in range(len(A) - 1):
        j = random.randrange(i, len(A))
        A[i], A[j] = A[j], A[i]


if __name__ == '__main__':
    import collections
    distributions = collections.defaultdict(int)

    for _ in itertools.repeat(None, 1000000):
        L = list(range(3))
        shuffle(L)
        distributions[tuple(L)] += 1

    for distribution, occurrences in distributions.items():
        print(distribution, occurrences)
