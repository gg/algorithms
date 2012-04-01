#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "util.h"


namespace util = algorithms::util;


// Shuffles elements in range [first, last) in-place using the Fischer-Yates
// algorithm.
//
// See: http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
template <typename RandomAccessIterator>
void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
    for (RandomAccessIterator i = first; i != last; i++) {
        int j = util::randint(last - i)();
        std::swap(*i, *(i + j));
    }
}


int main(int argc, char** argv) {
    srand(time(NULL));

    std::map<std::string, int> distributions;

    int arr[] = {0, 1, 2};

    for (int trial = 0; trial < 1000000; trial++) {
        std::vector<int> L(arr, arr + (sizeof arr / sizeof arr[0]));
        shuffle(L.begin(), L.end());
        std::stringstream ss;
        for (std::vector<int>::iterator it = L.begin(); it != L.end(); it++)
            ss << *it << " ";
        std::string dist = ss.str();
        if (distributions.count(dist) == 0)
            distributions[dist] = 0;
        distributions[dist]++;
    }

    std::map<std::string, int>::iterator it = distributions.begin();
    for (; it != distributions.end(); it++) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}
