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

// Fischer-Yates shuffle

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
