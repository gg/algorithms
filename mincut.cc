// Copyright (c) 2012 Gregg Gajic <gregg.gajic@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

// Minimum cut
// See: http://en.wikipedia.org/wiki/Cut_(graph_theory)#Minimum_cut'''

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"


namespace util = algorithms::util;


typedef std::string Vertex;
typedef std::pair<Vertex, Vertex> Edge;



// Given an undirected graph consisting of n vertices and m edges,
// returns a *possible* min cut after n - 1 edge contractions, where edges are
// chosen uniformly at random (Karger's algorithm).
//
// See: http://en.wikipedia.org/wiki/Karger%27s_algorithm
std::vector<Edge> randomized_contraction(const std::vector<Vertex>& vertices,
                                         const std::vector<Edge>& edges) {
    std::vector<Edge> contracted_edges(edges);

    for (int i = 0; i < vertices.size() - 2; i++) {
        int rand_edge_index = util::randint(contracted_edges.size())();
        Edge rand_edge = contracted_edges[rand_edge_index];

        Vertex super_vertex = "(" + rand_edge.first + ", " + rand_edge.second
                              + ")";

        std::vector<Edge>::iterator i = contracted_edges.begin();
        while (i != contracted_edges.end()) {
            if (i->first == rand_edge.first || i->first == rand_edge.second)
                i->first = super_vertex;
            if (i->second == rand_edge.first || i->second == rand_edge.second)
                i->second = super_vertex;
            // remove self-loops
            if (i->first == i->second)
                i = contracted_edges.erase(i);
            else
                i++;
        }
    }

    return contracted_edges;
}


void test_randomized_contraction() {
    const std::string adj_list =
         "1 19 15 36 23 18 39\n"
         "2 36 23 4 18 26 9\n"
         "3 35 6 16 11\n"
         "4 23 2 18 24\n"
         "5 14 8 29 21\n"
         "6 34 35 3 16\n"
         "7 30 33 38 28\n"
         "8 12 14 5 29 31\n"
         "9 39 13 20 10 17 2\n"
         "10 9 20 12 14 29\n"
         "11 3 16 30 33 26\n"
         "12 20 10 14 8\n"
         "13 24 39 9 20\n"
         "14 10 12 8 5\n"
         "15 26 19 1 36\n"
         "16 6 3 11 30 17 35 32\n"
         "17 38 28 32 40 9 16\n"
         "18 2 4 24 39 1\n"
         "19 27 26 15 1\n"
         "20 13 9 10 12\n"
         "21 5 29 25 37\n"
         "22 32 40 34 35\n"
         "23 1 36 2 4\n"
         "24 4 18 39 13\n"
         "25 29 21 37 31\n"
         "26 31 27 19 15 11 2\n"
         "27 37 31 26 19 29\n"
         "28 7 38 17 32\n"
         "29 8 5 21 25 10 27\n"
         "30 16 11 33 7 37\n"
         "31 25 37 27 26 8\n"
         "32 28 17 40 22 16\n"
         "33 11 30 7 38\n"
         "34 40 22 35 6\n"
         "35 22 34 6 3 16\n"
         "36 15 1 23 2\n"
         "37 21 25 31 27 30\n"
         "38 33 7 28 17 40\n"
         "39 18 24 13 9 1\n"
         "40 17 32 22 34 38\n";

    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    std::vector<std::string> rows = util::split(adj_list, '\n');
    typedef std::vector<std::string>::iterator Iter;

    for (Iter row = rows.begin(); row != rows.end(); row++) {
        std::vector<std::string> cols = util::split(*row, ' ');

        Vertex vertex = util::trim(cols[0]);
        vertices.push_back(vertex);

        int vertex_id;
        std::stringstream(vertex) >> vertex_id;

        for (Iter col = cols.begin() + 1; col != cols.end(); col++) {
            std::string endpoint = *col;
 
            // ignore edge permutations i.e. (1, 2) and (2, 1)
            int endpoint_id;
            std::stringstream(endpoint) >> endpoint_id;
            if (vertex_id > endpoint_id)
                continue;

            edges.push_back(Edge(vertex, endpoint));
        }
    }

    std::vector<Edge> min_cut(edges);
    const int n = vertices.size();
    const int trials = std::log(n) * std::pow(n, 2);
    for (int i = 0; i < trials; i++) {
        std::vector<Edge> possible_min_cut = randomized_contraction(vertices,
                                                                    edges);
        if (possible_min_cut.size() < min_cut.size()) {
            min_cut = possible_min_cut;
        }
    }

    assert(min_cut.size() == 3);
}


int main(int argc, char** argv) {
    srand(time(NULL));
    test_randomized_contraction();
    std::cout << "Tests passed." << std::endl;
    return 0;
}
