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

'''Merge sort'''

from __future__ import division
from __future__ import print_function


def mergesort(seq):
    n = len(seq)

    if n < 2:
        return seq

    left = seq[0:n // 2]
    right = seq[n // 2:]
    return merge(mergesort(left), mergesort(right))


def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    # When we reach the end of either subsequence, copy the remaining portion
    # to the result. Since both subsequences are already in sorted order, we do
    # not have to reorder the remaining portion.
    result.extend(left[i:] + right[j:])

    return result


def test_mergesort():
    import random
    for _ in range(1000):
        seq = [random.randint(0, 99) for i in range(1000)]
        assert(mergesort(seq[:]) == list(sorted(seq)))
    print('Tests passed.')


if __name__ == '__main__':
    test_mergesort()
