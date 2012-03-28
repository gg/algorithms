'''Solves the problem of computing the ith smallest element of an input array
(e.g. the median) in linear time.'''

import random
import quicksort


def randomized_selection(A, i, first=None, last=None):
    '''Returns the `i`th (indexed from 0) smallest element in `A[first:last]`.

    >>> randomized_selection([3, 5, 7, 1, 9, 4, 6, 8], 4)
    6
    
    Requires `len(A) > i`.
    
    `A` must not contain duplicate elements. 

    Recursively partitions `A`, as in randomized quicksort, where the pivot is
    chosen uniformly at random.
    '''
    first = first if first is not None else 0
    last = last if last is not None else len(A)

    if last - first == 1:
        return A[first]

    pivot_index, _ = quicksort.partition(A, first, last,
                                         quicksort.random_pivot)
    normalized_pivot_index = pivot_index - first
    if normalized_pivot_index == i:
        return A[pivot_index]
    elif normalized_pivot_index > i:
        return randomized_selection(A, i, first, pivot_index)
    else:
        return randomized_selection(A, i - normalized_pivot_index - 1,
                                    pivot_index + 1, last)


def main():
    import doctest
    failure_count, test_count = doctest.testmod()

    if failure_count > 0:
        return

    A = list(range(1000))
    for _ in range(1000):
        random.shuffle(A)
        i = random.randrange(len(A))
        assert(randomized_selection(A, i) == sorted(A)[i])
    print('Tests passed.')


if __name__ == '__main__':
    main()
