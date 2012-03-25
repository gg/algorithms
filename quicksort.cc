#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "util.h"


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
        return first + randint(last - first)();
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


template <typename Container, typename ChoosePivot>
void test_quicksort(const Container& container, ChoosePivot choose_pivot) {
    Container sorted(container);
    std::sort(sorted.begin(), sorted.end());

    Container actual(container);
    quicksort(actual.begin(), actual.end(), choose_pivot);

    assert(sequences_are_equal(actual, sorted));
}


int main(int argc, char** argv) {
    srand(time(NULL));

    std::vector<int> rand_seq(10000);
    std::generate_n(rand_seq.begin(), rand_seq.size(), randint(1000));

    std::vector<int> sorted_seq(rand_seq);
    std::sort(sorted_seq.begin(), sorted_seq.end());

    std::vector<std::vector<int> > inputs;
    inputs.push_back(rand_seq);
    inputs.push_back(sorted_seq);

    std::vector<std::vector<int> >::iterator i;
    for (i = inputs.begin(); i != inputs.end(); i++) {
        test_quicksort(*i, First());
        test_quicksort(*i, Last());
        test_quicksort(*i, Random());
        test_quicksort(*i, MedianOfThree());
    }

    std::cout << "Tests passed." << std::endl;
    return 0;
}
