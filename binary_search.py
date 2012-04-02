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

'''Binary search'''

from __future__ import division
from __future__ import print_function


def recursive_binary_search(sorted_sequence, key, start=0, end=None):
    '''Returns the index of `key` in the `sorted_sequence`, or None if `key` is
    not in `sorted_sequence`.'''
    end = end or len(sorted_sequence)

    if (end - start) < 0:
        return None

    middle = start + ((end - start) // 2)

    if sorted_sequence[middle] == key:
        return middle
    elif sorted_sequence[middle] < key:
        return recursive_binary_search(sorted_sequence, key, middle + 1, end)
    else:
        return recursive_binary_search(sorted_sequence, key, start, middle - 1)


def iterative_binary_search(sorted_sequence, key):
    '''Returns the index of `key` in the `sorted_sequence`, or None if `key` is
    not in `sorted_sequence`.'''
    start = 0
    end = len(sorted_sequence)

    while start < end:
        middle = start + ((end - start) // 2)
        if sorted_sequence[middle] == key:
            return middle
        elif sorted_sequence[middle] < key:
            start = middle + 1
        else:
            end = middle

    return None


if __name__ == '__main__':
    seq = [1, 1, 2, 5, 9, 11, 11, 11, 12, 18, 29, 37, 38, 40, 67, 78, 94, 94]

    assert(recursive_binary_search(seq, 12) == 8)
    assert(recursive_binary_search(seq, 13) == None)

    assert(iterative_binary_search(seq, 12) == 8)
    assert(iterative_binary_search(seq, 13) == None)

    print('Tests passed.')
