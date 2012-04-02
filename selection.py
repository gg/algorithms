# coding: utf-8

# Copyright (c) 2012 Gregg Gajic <gregg.gajic@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

'''Select the ith smallest element of an input array (e.g. the median) in
linear time.
'''

from __future__ import print_function

import random
import quicksort


def randomized_selection(A, i, first=None, last=None):
    '''Returns the `i`th smallest element (indexed from 0) in `A[first:last]`.

    Requires `len(A) > i`.

    `A` must not contain duplicate elements.

    Recursively partitions `A`, as in randomized quicksort, where the pivot is
    chosen uniformly at random.

    >>> randomized_selection([3, 5, 7, 1, 9, 4, 6, 8], 4)
    6
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
