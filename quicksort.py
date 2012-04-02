# coding: utf-8
'''Quicksort'''

from __future__ import division
from __future__ import print_function

from functools import partial
import math
import random


def random_pivot(A, first, last):
    '''Given a partition `A[first:last]` returns an index in range [`first`,
    `last`), chosen uniformly at random.
    '''
    return random.randrange(first, last)


def median_of_three(A, first, last):
    '''Given a partition `A[first:last]`, returns the index of the median of
    the first, middle, and last items of the partition, where
    middle = first + (last - first) // 2.

    As an optimization, the first, middle, and last items are sorted relative
    to each other (A[first] <= A[middle] <= A[last - 1]), thus the middle index
    is always returned.

    Choosing a median pivot in this way has its advantages:
      - Allows quicksort to run in O(nlogn) when `A` is (mostly) sorted.
      - Makes it more difficult to manipulate `A` such that quicksort runs in
        worst-case O(n^2).
      - Is is generally faster than using a pseudo-random number generator.
    '''
    last = last - 1
    middle = first + (last - first) // 2

    # For added benefit, the first, middle, and last items of `A` in range
    # [`first`, `last`) are sorted with respect to each other, such that:
    #
    #     `A[first] <= A[middle] <= A[last - 1]`
    #
    # This makes it more difficult ot manipulate `A` such that quickosort runs
    # in worst-case O(n^2).
    if A[first] > A[middle]:
        A[first], A[middle] = A[middle], A[first]

    if A[middle] > A[last]:
        A[middle], A[last] = A[last], A[middle]
        if A[middle] < A[first]:
            A[first], A[middle] = A[middle], A[first]

    assert(A[first] <= A[middle] and A[middle] <= A[last])
    return middle


def partition(A, first, last, choose_pivot):
    '''Partitions the items of `A[first:last]` about a pivot chosen by
    `choose_pivot` as follows:

        [    <    i][   p][     >    ]
      first                         last

    [    <    i] contains items less than the pivot, with i indexing the
    rightmost item in this section.

    [   p] contains items equal to the pivot, with p indexing the rightmost
    item in this section.

    [     >    ] contains items greater than the pivot.

    Returns a pair of indices corresponding to the first and last items of the
    pivot section, `i + 1` and `p` respectively.

    If `choose_pivot` runs in O(1) time, this algorithm runs in O(n) time.
    Also note that irrespective of how the pivot is chosen, quicksort runs in
    O(nlogn) *expected time* (averaged over all permutation of `A`).
    '''
    pivot_index = choose_pivot(A, first, last)

    # Swap pivot to first of A
    A[first], A[pivot_index] = A[pivot_index], A[first]
    pivot = A[first]

    # Each iteration, we partition A as follows:
    #     [   p][  <  i][  >  j][     ?     ]
    #   first                              last
    #
    # - [   p] contains items equal to the pivot, with p indexing the
    #   rightmost item in this section.
    # - [  <  i] contains items less than the pivot, with i indexing the
    #   rightmost item in this section.
    # - [  >  j] contains items greater than the pivot, with j indexing
    #   the rightmost item in this section.
    # - [     ?     ] contains unpartitioned items.
    #
    # When the loop terminates, A will be partitioned as follows:
    #     [   p][    <    i][    >    j]
    #   first                         last
    p = i = first
    for j in range(first + 1, last):
        if A[j] <= pivot:
            i += 1
            A[j], A[i] = A[i], A[j]
            if A[i] == pivot:
                p += 1
                A[i], A[p] = A[p], A[i]

    # Swap all the items equal to the pivot from the first of A with
    # the rightmost items that are less than the pivot (indexed by i).
    #
    # After the swaps, A will be partitioned as follows:
    #     [    <    i][   p][     >    ]
    #   first                         last
    p_new = i
    for pivot_index in range(first, p + 1):
        A[pivot_index], A[i] = A[i], A[pivot_index]
        i -= 1
    p = p_new

    return i + 1, p


def quicksort(A, first=None, last=None, choose_pivot=median_of_three):
    '''Sorts `A[first:last]` in-place by recursively partitioning items
    about pivots chosen by a `choose_pivot` callable.

    `choose_pivot` is passed 3 parameters: (`A`, `first`, `last`),
    corresponding to the partition `A[first:last]`, and must return an
    integer index in range `[first, last)`.

    Returns the estimated total number of comparisons made by each recursive
    call. Since the number of comparisons depends on which items are chosen as
    pivots, you can use this to compare the relative performance of various
    `choose_pivot` implementations.
    '''
    first = first if first is not None else 0
    last = last if last is not None else len(A)

    if last - first < 2:
        return 0

    # The number of comparisons is estimated to be (last - first - 1),
    # since the chosen pivot will be compared with each of the other
    # (last - first - 1) items.
    pivot_first, pivot_last = partition(A, first, last, choose_pivot)
    comparisons = last - first - 1

    return (comparisons
            + quicksort(A, first, pivot_first, choose_pivot)
            + quicksort(A, pivot_last + 1, last, choose_pivot))


if __name__ == '__main__':
    random_input = ('random sequence of 100000 ints',
                    [random.randrange(10000) for _ in range(100000)])
    sorted_input = ('sorted sequence of 100000 ints', list(range(100000)))

    choose_pivot_funcs = [('first', lambda A, first, last: first),
                          ('last', lambda A, first, last: last - 1),
                          ('random', random_pivot),
                          ('median_of_three', median_of_three)]

    for input_name, input_array in (random_input, sorted_input):
        print('==>', input_name)

        headers = ('choose_pivot', 'comparisons')
        fmt = '    {:<18}'
        print(''.join(fmt.format(header) for header in headers))
        print(''.join(fmt.format('-' * len(header)) for header in headers))

        for pivot_func_name, pivot_func in choose_pivot_funcs:
            A = input_array[:]
            try:
                comparisons = quicksort(A, choose_pivot=pivot_func)
                assert(A == sorted(A))
            except RuntimeError as e:
                comparisons = e

            print(''.join(fmt.format(value) for value in (pivot_func_name,
                                                          comparisons)))
        print()
