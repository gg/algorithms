#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "util.h"


struct NaivePivot {
    // Given a range of items [first, last), naively selects first as the
    // pivot. While this works well when the items items in the input sequence
    // are unordered, it causes quicksort to run in worst-case O(n^2) when the
    // input sequence is (mostly) sorted.
    template<typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        return first;
    }
};


struct MedianPivot {
    // Given a range of items [first, last), returns an iterator to the median
    // of *first, *(last - 1), and *middle, where middle = (first - last) / 2.
    //
    // As an optimization, first, last, and middle are sorted relative to each
    // other (*first <= *middle <= *(last - 1)), thus middle is always
    // returned.
    // 
    // Choosing a median pivot in this way has its advantages:
    //  - Allows quicksort to run in O(nlogn) when `A` is (mostly) sorted.
    //  - Makes it more difficult to manipulate `A` such that quicksort runs in
    //    worst-case O(n^2).
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


struct RandomPivot {
    // Returns an iterator to a random item in range [first, last). Selecting
    // a random pivot allows quicksort to run in O(nlogn) expected time when
    // A is (mostly) sorted.
    template <typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first,
                                    RandomAccessIterator last) {
        return first + randint(last - first)();
    }
};


// Partitions the input sequence in range [`first`, `last`) about a pivot
// chosen by `choose_pivot` as follows:
//    
//    [    <    i][   p][     >    ]
//  first                         last
//
// - [    <    i] contains items less than the pivot, where i is an iterator
//   to the rightmost item in this section.
// - [   p] contains items equal to the pivot, where p is an iterator to the
//   rightmost item in this section.
// - [     >    ] contains items greater than the pivot.
//
// Returns a pair of iterators corresponding to the first and last items the
// pivot section, `i + 1` and `p` respectively.
//
// If `choose_pivot` runs in O(1) time, this algorithm runs in O(n) time.
// Also note that irrespective of how the pivot is chosen, quicksort runs in
// O(nlogn) *expected time* (averaged over all permutation of the input
// sequence).
template <typename RandomAccessIterator, typename ChoosePivot>
std::pair<RandomAccessIterator, RandomAccessIterator> partition_section(
        RandomAccessIterator first, RandomAccessIterator last,
        ChoosePivot choose_pivot) {
    RandomAccessIterator p = choose_pivot(first, last);
    std::swap(*p, *first);
    p = first;

    // Each iteration, we advance i and j, partitioning the input sequence as
    // follows:
    //     [   p][  <  i][  >  j][     ?     ]
    //   first                              last
    // 
    // - [   p] contains items equal to the pivot, with p indexing the
    //   rightmost item in this section.
    // - [  <  i] contains items less than the pivot, with i indexing the
    //   rightmost item in this section.
    // - [  >  j] contains items greater than the pivot, with j indexing
    //   the rightmost item in this section.
    // - [     ?     ] contains unpartitioned items.
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
    quicksort(first, last, MedianPivot());
}


void test_quicksort() {
    srand(time(NULL));

    std::vector<int> A1(100001);
    std::generate_n(A1.begin(), A1.size(), randint(1000));
    std::vector<int> A2(A1);
    std::vector<int> A3(A1);

    std::vector<int> A_sorted(A1);
    std::sort(A_sorted.begin(), A_sorted.end());

    quicksort(A1.begin(), A1.end(), NaivePivot());
    assert(sequences_are_equal(A1, A_sorted));

    quicksort(A2.begin(), A2.end(), RandomPivot());
    assert(sequences_are_equal(A2, A_sorted));

    quicksort(A3.begin(), A3.end(), MedianPivot());
    assert(sequences_are_equal(A3, A_sorted));

    std::cout << "Tests passed." << std::endl;
}


int main(int argc, char** argv) {
    test_quicksort();
    return 0;
}
