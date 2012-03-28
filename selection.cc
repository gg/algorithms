// Solves the problem of computing the ith smallest element of an input array
// (e.g. the median) in linear time.

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
