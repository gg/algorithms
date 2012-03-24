'''Quicksort algorithm.'''

from __future__ import division
from functools import partial
import random


def naive_pivot(A, first, last):
    """Given a partition of `A` in range [`first`, `last`), naively returns
    the leftmost `first` as the pivot index. While this works well when the
    items in `A` are randomly ordered, it causes quicksort to run in worst-case
    O(n^2) when `A` is (mostly) sorted.
    """
    return first


def random_pivot(A, first, last):
    '''Given a partition of `A` in range [`first`, `last`), returns a pivot 
    index uniformly at random. Selecting a random pivot allows quicksort to run
    in O(nlogn) expected time when `A` is (mostly) sorted.
    '''
    return random.randrange(first, last)


def median_pivot(A, first, last):
    """Given a partition of `A` in range [`first`, `last`), sorts `A[first]`,
    `A[middle]`, `A[last - 1]` such that `A[first] <= A[middle] <= A[last]` and
    returns `middle` as the pivot index, where
    `middle = (first + (last - first) // 2)`.
    
    Choosing a median pivot in this way has its advantages:
       - Allows quicksort to run in O(nlogn) when `A` is (mostly) sorted.
       - Makes it more difficult to manipulate `A` such that quicksort runs in
         worst-case O(n^2).
       - Is is generally faster than using a pseudo-random number generator.
    """
    last = last - 1
    middle = first + ((last - first) // 2)

    if A[first] > A[middle]:
        A[first], A[middle] = A[middle], A[first]

    if A[middle] > A[last]:
        A[middle], A[last] = A[last], A[middle]
        if A[middle] < A[first]:
            A[first], A[middle] = A[middle], A[first]

    assert(A[first] <= A[middle] and A[middle] <= A[last])
    return middle


def partition(A, first, last, choose_pivot):
    '''Partitions the items of A in range [`first`, `last`) about a pivot
    chosen by `choose_pivot` as follows:
        
        [    <    i][   p][     >    ]
      first                         last

    - [    <    i] contains items less than the pivot, with i indexing the
      rightmost item in this section.
    - [   p] contains items equal to the pivot, with p indexing the
      rightmost item in this section.
    - [     >    ] contains items greater than the pivot.

    Returns a pair of indexes corresponding to the first and last items of the
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


def quicksort(A, first=None, last=None, choose_pivot=median_pivot):
    first = first if first is not None else 0
    last = last if last is not None else len(A)
    if last - first < 2:
        return
    pivot_first, pivot_last = partition(A, first, last, choose_pivot)
    quicksort(A, first, pivot_first, choose_pivot)
    quicksort(A, pivot_last + 1, last, choose_pivot)


naive_quicksort = partial(quicksort, choose_pivot=naive_pivot)


randomized_quicksort = partial(quicksort, choose_pivot=random_pivot)


median_quicksort = partial(quicksort, choose_pivot=median_pivot)


RAND_SEQUENCE = [random.randrange(10000) for _ in range(100000)]


def test_quicksort(quicksort_func, A):
    expected = sorted(A)
    quicksort_func(A)
    assert(A == expected)


if __name__ == '__main__':
    from timeit import Timer

    tests = ['test_quicksort(naive_quicksort, RAND_SEQUENCE[:])',
             'test_quicksort(randomized_quicksort, RAND_SEQUENCE[:])',
             'test_quicksort(median_quicksort, RAND_SEQUENCE[:])',
             'test_quicksort(naive_quicksort, sorted(RAND_SEQUENCE))',
             'test_quicksort(randomized_quicksort, sorted(RAND_SEQUENCE))',
             'test_quicksort(median_quicksort, sorted(RAND_SEQUENCE))']

    for test in tests:
        imports = ','.join(['test_quicksort', 'naive_quicksort',
                            'randomized_quicksort', 'median_quicksort',
                            'RAND_SEQUENCE'])
        t = Timer(test, 'from __main__ import ' + imports)
        result = None
        try:
            result = t.timeit(5)
        except RuntimeError as e:
            result = e
        finally:
            print(test[14:], '  ', result)