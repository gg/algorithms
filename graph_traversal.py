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

'''Graph traversal: BFS, DFS, connected components, topological sort.'''

from __future__ import print_function
from __future__ import unicode_literals

from collections import defaultdict, deque


def depth_limited_search(graph, vertex, goal, depth, path=[]):
    if depth == 0 and vertex == goal:
        yield path
    elif depth > 0:
        for adjacent_vertex in graph[vertex]:
            for path in depth_limited_search(graph, adjacent_vertex, goal,
                                             depth - 1,
                                             path + [adjacent_vertex]):
                yield path


def iterative_deepening_DFS(graph, start_vertex, goal):
    depth = 0
    while depth < len(graph):
        for path in depth_limited_search(graph, start_vertex, goal, depth,
                                         path=[start_vertex]):
            return path
        depth += 1


def BFS(graph, start_vertex, goal=None):
    connected_component = set()
    unvisited = deque([start_vertex])
    while unvisited:
        vertex = unvisited.pop()
        connected_component.add(vertex)
        if goal is not None and vertex == goal:
            break
        for adjacent_vertex in graph[vertex]:
            if not adjacent_vertex in connected_component:
                connected_component.add(adjacent_vertex)
                unvisited.appendleft(adjacent_vertex)
    return connected_component


def iterative_DFS(graph, start_vertex, goal=None):
    connected_component = set()
    unvisited = [start_vertex]
    while unvisited:
        vertex = unvisited.pop()
        connected_component.add(vertex)
        if goal is not None and vertex == goal:
            break
        for adjacent_vertex in graph[vertex]:
            if not adjacent_vertex in connected_component:
                connected_component.add(adjacent_vertex)
                unvisited.append(adjacent_vertex)
    return connected_component


def connected_components(undirected_graph):
    connected_components = set()
    for vertex in undirected_graph:
        if any(vertex in c for c in connected_components):
            continue
        connected_component = BFS(undirected_graph, vertex)
        connected_components.add(frozenset(connected_component))
    return connected_components


def topological_sort(dag):
    _, finishing_times, _ = DFS_loop(dag)
    return sorted(finishing_times, key=lambda v: -finishing_times[v])


def DFS(graph, start_vertex, visited=None):
    visited = visited if visited is not None else set()
    connected_component = set()
    finishing_order = []

    def visit(vertex):
        visited.add(vertex)
        connected_component.add(vertex)
        for adjacent_vertex in graph[vertex]:
            if adjacent_vertex not in visited:
                connected_component.add(adjacent_vertex)
                visit(adjacent_vertex)
        finishing_order.append(vertex)

    visit(start_vertex)

    return connected_component, finishing_order


def DFS_loop(graph, key=None):
    connected_components = set()
    finishing_times = {}
    t = 1
    leaders = {}
    visited = set()

    for vertex in sorted(graph, key=key):
        if vertex in visited:
            continue
        connected_component, finishing_order = DFS(graph, vertex, visited)
        for v in connected_component:
            leaders[v] = vertex
        for v in finishing_order:
            finishing_times[v] = t
            t += 1
        connected_components.add(frozenset(connected_component))

    return connected_components, finishing_times, leaders


def strongly_connected_components(directed_graph):
    '''Kosaraju's two-pass algorithm
    http://en.wikipedia.org/wiki/Kosaraju%27s_algorithm
    '''
    sccs = []

    reversed_graph = defaultdict(set)
    for vertex in directed_graph:
        for adjacent_vertex in directed_graph[vertex]:
            reversed_graph[adjacent_vertex].add(vertex)

    _, finishing_times, _ = DFS_loop(reversed_graph)

    finishing_time_descending = lambda vertex: -finishing_times[vertex]

    connected_components, _, _ = DFS_loop(directed_graph,
                                          key=finishing_time_descending)
    return connected_components


def dijkstra_shortest_path(graph, start_vertex):
    '''Dijkstra's shortest path algorithm'''
    shortest_distances = {start_vertex: 0}
    while True:
        edges = ((from_v, to_v, shortest_distances[from_v] + weight)
                 for from_v in shortest_distances
                 for to_v, weight in graph[from_v]
                 if to_v not in shortest_distances)
        try:
            min_edge = min(edges, key=lambda edge: edge[2])
            from_v, to_v, distance = min_edge
            shortest_distances[to_v] = distance
        except ValueError: # raised by min() when no more edges to consider
            return shortest_distances


if __name__ == '__main__':
    graph = {'s': ['a', 'b'],
             'a': ['c', 's'],
             'b': ['d', 's'],
             'c': ['a', 'e'],
             'd': ['b', 'e'],
             'e': ['c', 'd'],
             'f': ['g'],
             'g': ['f']}

    components = [{'s': ['a', 'b'],
                   'a': ['c', 's'],
                   'b': ['d', 's'],
                   'c': ['a', 'e'],
                   'd': ['b', 'e'],
                   'e': ['c', 'd']},
                  {'f': ['g'], 'g': ['f']}]

    for search_func in (BFS, lambda graph, start: DFS(graph, start)[0],
                        iterative_DFS):
        assert(search_func(graph, 's') == set(components[0].keys()))
        assert(search_func(graph, 'f') == set(components[1].keys()))

    assert(iterative_deepening_DFS(graph, 's', 'd') == ['s', 'b', 'd'])
    assert(iterative_deepening_DFS(graph, 's', 'f') == None)

    assert(connected_components(graph) ==
           set(frozenset(component.keys()) for component in components))

    digraph1 = {'s': ['v', 'w'],
                      'v': ['t'],
                      'w': ['t'],
                      't': []}

    assert(list(topological_sort(digraph1)) == list('swvt'))

    assert(strongly_connected_components(digraph1) ==
           set([frozenset([vertex]) for vertex in 'swvt']))

    digraph2 = {'a': ['b'],
                'b': ['c', 'h'],
                'c': ['d', 'f'],
                'd': ['e'],
                'e': ['f'],
                'f': ['d'],
                'g': ['f', 'i', 'k'],
                'h': ['a', 'g', 'j'],
                'i': ['e', 'k'],
                'j': ['g'],
                'k': ['j']}

    assert(strongly_connected_components(digraph2) ==
           set([frozenset(lst) for lst in (['a', 'b', 'h'],
                                           ['c'],
                                           ['d', 'e', 'f'],
                                           ['g', 'i', 'j', 'k'])]))

    digraph3 = {2: [],
                3: [8, 10],
                5: [11],
                7: [8, 11],
                8: [9],
                9: [],
                10: [],
                11: [2, 9]}

    # Example from http://en.wikipedia.org/wiki/Topological_sorting#Examples
    assert(list(topological_sort(digraph3)) == [7, 5, 11, 3, 10, 8, 9, 2])


    weighted_digraph = {'s': [('v', 1), ('w', 4)],
                        'v': [('w', 2), ('t', 6)],
                        'w': [('t', 3)],
                        't': []}
    assert(dijkstra_shortest_path(weighted_digraph, 's') ==
           {'s': 0, 'v': 1, 'w': 3, 't': 6})

    print('Tests passed.')
