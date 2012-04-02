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

'''Fischer-Yates shuffle'''

from __future__ import print_function

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
