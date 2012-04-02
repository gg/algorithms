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

// Select the ith smallest element of an input array (e.g. the median) in
// linear time.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "quicksort.h"
#include "util.h"


namespace quicksort = algorithms::quicksort;
namespace util = algorithms::util;


// Returns the ith smallest element (indexed from 0) in range [first, last).
template <typename RandomAccessIterator>
typename RandomAccessIterator::value_type randomized_selection(
        int i, RandomAccessIterator first, RandomAccessIterator last) {
    if (last - first == 1)
        return *first;

    std::pair<RandomAccessIterator, RandomAccessIterator> pivot_range; 
    pivot_range = quicksort::partition_section(first, last,
                                               quicksort::Random());

    int normalized_pivot_index = pivot_range.first - first;

    if (normalized_pivot_index == i) {
        return *pivot_range.first; 
    }
    else if (normalized_pivot_index > i) {
        return randomized_selection(i, first, pivot_range.first);
    }
    else { // normalized_pivot_index < i
        return randomized_selection(i - normalized_pivot_index - 1,
                                    pivot_range.first + 1,
                                    last);
    }
}


int main(int argc, char** argv) {
    std::vector<int> A(1000);
    for (int i = 0; i < 1000; i++) {
        A.push_back(i);
    }

    std::vector<int> A_sorted(A);

    for (int trial = 0; trial < 1000; trial++) {
        std::random_shuffle(A.begin(), A.end());
        int i = util::randint(A.size())();
        assert(randomized_selection(i, A.begin(), A.end()) == A_sorted[i]);
    }

    std::cout << "Tests passed." << std::endl;
    return 0;
}
