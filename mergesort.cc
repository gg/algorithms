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

// Merge sort

#include <algorithm>
#include <cassert>
#include <vector>
#include <ctime>

#include "util.h"


namespace util = algorithms::util;


template <typename Container>
Container merge(const Container& left,
                const Container& right) {
    Container result;

    typename Container::const_iterator i, j;
    i = left.begin();
    j = right.begin();

    while (i != left.end() && j != right.end()) {
        if (*i <= *j) {
            result.push_back(*i++);
        }
        else {
            result.push_back(*j++);
        }
    }

    // When we reach the end of either subsequence, copy the remaining portion
    // to the result. Since both subsequences are already in sorted order, we
    // do not have to reorder the remaining portion.
    std::copy(i, left.end(), std::back_inserter(result));
    std::copy(j, right.end(), std::back_inserter(result));

    return result;
}


template <typename Container>
Container mergesort(const Container& seq) {
    const std::size_t n = seq.size();

    if (n <= 1) {
        return seq;
    }

    typename Container::const_iterator middle = seq.begin() + n / 2;
    Container left(seq.begin(), middle);
    Container right(middle, seq.end());

    return merge(mergesort(left), mergesort(right));
}


void test_mergesort() {
    typedef std::vector<int> Vector;
    Vector seq(1000);
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        std::generate_n(seq.begin(), seq.size(), util::randint(100));
        Vector sorted(seq.begin(), seq.end());
        std::sort(sorted.begin(), sorted.end());
        assert(util::sequences_are_equal(mergesort(seq), sorted));
    }
}


int main(int argc, char** argv) {
    test_mergesort();
}
