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

// Binary search

#include <cassert>
#include <vector>


template <class RandomAccessIterator, class KeyValue, class DefaultValue>
DefaultValue recursive_binary_search(
        RandomAccessIterator start, RandomAccessIterator end, KeyValue key,
        DefaultValue default_value) {
    if (start == end) {
        return default_value;
    }

    RandomAccessIterator middle = start + ((end - start) / 2);

    if (*middle == key) {
        return middle;
    }
    else if (*middle < key) {
        return recursive_binary_search(middle + 1, end, key, default_value);
    }
    else {
        return recursive_binary_search(start, middle, key, default_value);
    }
}


template <class RandomAccessIterator, class Value>
RandomAccessIterator recursive_binary_search(RandomAccessIterator first,
                                             RandomAccessIterator last,
                                             Value key) {
    return recursive_binary_search(first, last, key, last);
}


template <class RandomAccessIterator, class Value>
RandomAccessIterator iterative_binary_search(RandomAccessIterator first,
                                             RandomAccessIterator last,
                                             Value key) {
    RandomAccessIterator start, end;
    start = first;
    end = last;

    while (start != end) {
        RandomAccessIterator middle = start + ((end - start) / 2);
        if (*middle == key) {
            return middle;
        }
        else if (*middle < key) {
            start = middle + 1;
        }
        else {
            end = middle;
        }
    }

    return last;
}



int main(int argc, char** argv) {
    int seq[] = {1, 1, 2, 5, 9, 11, 11, 11, 12, 18, 29, 37, 38, 40, 67, 78, 94,
                 94};

    std::vector<int> sorted_seq(seq, seq + (sizeof seq / sizeof seq[0]));

    assert(recursive_binary_search(sorted_seq.begin(), sorted_seq.end(), 12)
           == sorted_seq.begin() + 8);
    assert(recursive_binary_search(sorted_seq.begin(), sorted_seq.end(), 13)
           == sorted_seq.end());

    assert(iterative_binary_search(sorted_seq.begin(), sorted_seq.end(), 12)
           == sorted_seq.begin() + 8);
    assert(iterative_binary_search(sorted_seq.begin(), sorted_seq.end(), 13)
            == sorted_seq.end());

    return 0;
}
