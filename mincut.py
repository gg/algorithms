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

'''Minimum cut
See: http://en.wikipedia.org/wiki/Cut_(graph_theory)#Minimum_cut'''

from __future__ import print_function
from __future__ import unicode_literals

import itertools
import io
import math
import random


def randomized_contraction(vertices, edges):
    '''Given an undirected graph consisting of n vertices and m edges,
    returns a *possible* min cut after n - 1 edge contractions, where edges are
    chosen uniformly at random (Karger's algorithm).

    See: http://en.wikipedia.org/wiki/Karger%27s_algorithm
    '''
    contracted_edges = edges[:]
    for _ in itertools.repeat(None, len(vertices) - 2):
        rand_edge = random.sample(contracted_edges, 1)[0]
        super_vertex = rand_edge

        i = 0
        while i < len(contracted_edges):
            (v, u) = contracted_edges[i]
            if v in rand_edge:
                v = super_vertex
            if u in rand_edge:
                u = super_vertex
            # remove self-loops
            if v == u:
                del contracted_edges[i]
            else:
                contracted_edges[i] = (v, u)
                i += 1
    return contracted_edges


if __name__ == '__main__':
    adj_list = '''\
     1    19    15    36    23  18  39
     2    36    23     4    18  26  9
     3    35     6    16    11
     4    23     2    18    24
     5    14     8    29    21
     6    34    35     3    16
     7    30    33    38    28
     8    12    14     5    29  31
     9    39    13    20    10  17  2
    10     9    20    12    14  29
    11     3    16    30    33  26
    12    20    10    14     8
    13    24    39     9    20
    14    10    12     8     5
    15    26    19     1    36
    16     6     3    11    30  17  35 32
    17    38    28    32    40  9  16
    18     2     4    24    39  1
    19    27    26    15     1
    20    13     9    10    12
    21     5    29    25    37
    22    32    40    34    35
    23     1    36     2     4
    24     4    18    39    13
    25    29    21    37    31
    26    31    27    19    15  11  2
    27    37    31    26    19  29
    28     7    38    17    32
    29     8     5    21    25  10  27
    30    16    11    33     7  37
    31    25    37    27    26  8
    32    28    17    40    22  16
    33    11    30     7    38
    34    40    22    35     6
    35    22    34     6     3  16
    36    15     1    23     2
    37    21    25    31    27  30
    38    33     7    28    17  40
    39    18    24    13     9  1
    40    17    32    22    34  38'''

    vertices = []
    edges = []
    for line in io.StringIO(adj_list):
        row = [int(val) for val in line.split()]
        vertex = row[0]
        vertices.append(vertex)
        for endpoint in row[1:]:
            # ignore edge permutations i.e. (1, 2) and (2, 1)
            if endpoint > vertex:
                edges.append((vertex, endpoint))

    min_cut = edges
    trials = int(math.log(len(vertices)) * len(vertices) ** 2)
    for _ in itertools.repeat(None, trials):
        possible_min_cut = randomized_contraction(vertices, edges)
        if len(possible_min_cut) < len(min_cut):
            min_cut = possible_min_cut
    assert(len(min_cut) == 3)

    print('Tests passed.')
