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

// Quicksort

#ifndef ALGORITHMS_QUICKSORT_H
#define ALGORITHMS_QUICKSORT_H

#include <algorithm>

#include "util.h"


namespace algorithms {
namespace quicksort {

struct First {
    // Given a pair of iterators, first and last, corresponding to items in
    // range [first, last), returns first.
    template<typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        return first;
    }
};


struct Last {
    // Given a pair of iterators, first and last, corresponding to items in
    // range [first, last), returns (last - 1).
    template<typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        return last - 1;
    }
};


struct MedianOfThree {
    // Given a pair of iterators, first and last, corresponding to items in
    // range [first, last), returns an iterator to the median
    // of *first, *(last - 1), and *middle, where
    // middle = first + (last - first) / 2.
    //
    // As an optimization, the first, last, and middle items are sorted
    // relative to each other (*first <= *middle <= *(last - 1)), thus middle
    // is always returned.
    // 
    // Choosing a median pivot in this way has its advantages:
    //  - Allows quicksort to run in O(nlogn) when the input sequence is
    //    (mostly) sorted.
    //  - Makes it more difficult to manipulate the input sequence such that
    //    quicksort runs in worst-case O(n^2).
    //  - Is is generally faster than using a pseudo-random number generator.
    template<typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        RandomAccessIterator middle = first + ((last - first) / 2);

        // Input range is [first, last), thus the last element is last - 1.
        last--;

        if (*first > *middle)
            std::swap(*first, *middle);

        if (*middle > *last) {
            std::swap(*middle, *last);

            if (*first > *middle)
                std::swap(*first, *middle);
        }

        return middle;
    }
};


struct Random {
    // Given a pair of iterators, first and last, corresponding to items in
    // range [first, last), returns an iterator to an item in
    // range [first, last) selected uniformly at random.
    template <typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        return first + util::randint(last - first)();
    }
};


// Given a pair of iterators, first and last, corresponding to items in range
// [first, last), partitions the items about a pivot chosen by `choose_pivot`
// as follows:
//    
//    [    <     ][   p][     >    ]
//  first                         last
//
// Where:
//  [    <     ] contains items less than the pivot
//  [   p] contains items equal to the pivot
//  [     >    ] contains items greater than the pivot.
//
// Returns a pair of iterators to the first and last items in [  p].
template <typename RandomAccessIterator, typename ChoosePivot>
std::pair<RandomAccessIterator, RandomAccessIterator> partition_section(
        RandomAccessIterator first, RandomAccessIterator last,
        ChoosePivot choose_pivot) {
    RandomAccessIterator p = choose_pivot(first, last);
    std::swap(*p, *first);
    p = first;

    // Each iteration, we advance i and j, partitioning the input sequence as
    // follows:
    //
    //     [   p][  <  i][  >  j][     ?     ]
    //   first                              last
    // 
    // Where:
    //
    //   [   p] contains items equal to the pivot, with p indexing the
    //   rightmost item in this section.
    //
    //   [  <  i] contains items less than the pivot, with i indexing the
    //   rightmost item in this section.
    //
    //   [  >  j] contains items greater than the pivot, with j indexing
    //   the rightmost item in this section.
    //
    //   [     ?     ] contains unpartitioned items.
    //
    //
    // When the loop terminates, the input sequence will be partitioned as
    // follows:
    //     [   p][    <    i][    >    ]j
    //   first                        last
    RandomAccessIterator i, j;
    i = first;
    for (j = first + 1; j != last; j++) {
        if (*j <= *p) {
            std::swap(*++i, *j);
            if (*i == *p)
                std::swap(*++p, *i);
        }
    }

    // Swap all the items equal to the pivot from the start of the input
    // sequence with the rightmost items that are less than the pivot.
    //
    // After the swaps, the input sequence will be partitioned as follows:
    //     [    <    i][   p][     >    ]
    //   first                         last
    RandomAccessIterator p_new = i;
    for (RandomAccessIterator piv = first; piv != p + 1; piv++)
        std::swap(*piv, *i--);
    p = p_new;

    return std::pair<RandomAccessIterator, RandomAccessIterator>(i + 1, p);
}


// Sorts items in range [first, last) in-place by recursively partitioning
// items about pivots chosen by a `choose_pivot` function/functor.
//
// `choose_pivot` is a passed a pair of iterators: (first, last),
// corresponding to a partition of items in range [first, last), and must
// return an iterator to an item in range [first, last).
template <typename RandomAccessIterator, typename ChoosePivot>
void quicksort(RandomAccessIterator first, RandomAccessIterator last,
               ChoosePivot choose_pivot) {
    if (last - first < 2)
        return;

    std::pair<RandomAccessIterator, RandomAccessIterator> pivot_range;
    pivot_range = partition_section(first, last, choose_pivot);
    quicksort(first, pivot_range.first, choose_pivot);
    quicksort(pivot_range.second + 1, last, choose_pivot);
}


template <typename RandomAccessIterator>
void quicksort(RandomAccessIterator first, RandomAccessIterator last) {
    quicksort(first, last, MedianOfThree());
}

} // namespace quicksort
} // namespace algorithms

#endif  // ALGORITHMS_QUICKSORT_H
