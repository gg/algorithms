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

// Counting array inversions

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "util.h"


namespace util = algorithms::util;


template <typename Container>
struct InversionResult {
    Container sorted;
    std::size_t inversions;

    InversionResult() : sorted(), inversions(0) {}

    InversionResult(const Container& sorted, std::size_t inversions)
        : sorted(sorted), inversions(inversions) {}
};


template <typename Container>
InversionResult<Container> merge_and_count_inversions(
        const Container& left, const Container& right) {
    InversionResult<Container> result;

    typename Container::const_iterator i, j;
    i = left.begin();
    j = right.begin();

    while (i != left.end() && j != right.end()) {
        if (*i <= *j) {
            result.sorted.push_back(*i++);
        }
        else {
            result.sorted.push_back(*j++);
            result.inversions += left.end() - i;
        }
    }

    std::copy(i, left.end(), std::back_inserter(result.sorted));
    std::copy(j, right.end(), std::back_inserter(result.sorted));

    return result;
}


template <typename Container>
InversionResult<Container> sort_and_count_inversions(const Container& seq) {
    typedef InversionResult<Container> Result;

    const std::size_t n = seq.size();

    if (n < 2) {
        return Result(seq, 0);
    }

    typename Container::const_iterator middle = seq.begin() + (n / 2);
    Result left = sort_and_count_inversions(Container(seq.begin(), middle));
    Result right = sort_and_count_inversions(Container(middle, seq.end()));

    Result merged = merge_and_count_inversions(left.sorted, right.sorted);

    std::size_t total_inversions = left.inversions + right.inversions
                                    + merged.inversions;

    return Result(merged.sorted, total_inversions);
}


template <typename Container>
void test_single_input(const Container& seq, std::size_t expected_inversions) {
    InversionResult<Container> result = sort_and_count_inversions(seq);

    Container sorted(seq.begin(), seq.end());
    std::sort(sorted.begin(), sorted.end());

    assert(util::sequences_are_equal(result.sorted, sorted));
    assert(result.inversions == expected_inversions);
}


void test_count_inversions() {
    typedef std::vector<int> Container;
    Container empty;
    test_single_input(empty, 0);

    int input2[] = {5};
    test_single_input(Container(input2, input2 + 1), 0);

    int input3[] = {5, 4};
    test_single_input(Container(input3, input3 + 2), 1);

    int input4[] = {4, 5};
    test_single_input(Container(input4, input4 + 2), 0);

    int input5[] = {2, 4, 1, 3, 5};
    test_single_input(Container(input5, input5 + 5), 3);

    int input6[] = {1, 2, 3, 4, 5, 6};
    test_single_input(Container(input6, input6 + 6), 0);

    int input7[] = {6, 5, 4, 3, 2, 1};
    test_single_input(Container(input7, input7 + 6), 15);
}


int main(int argc, char** argv) {
    test_count_inversions();

    std::ifstream input("IntegerArray.txt");
    std::vector<int> seq;
    int num;
    while (input >> num) {
        seq.push_back(num);
    }
    test_single_input(seq, 2407905288);

    return 0;
}
