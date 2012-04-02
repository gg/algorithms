// Quicksort

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "quicksort.h"
#include "util.h"


namespace quicksort = algorithms::quicksort;
namespace util = algorithms::util;


template <typename Container, typename ChoosePivot>
void test_quicksort(const Container& container, ChoosePivot choose_pivot) {
    Container sorted(container);
    std::sort(sorted.begin(), sorted.end());

    Container actual(container);
    quicksort::quicksort(actual.begin(), actual.end(), choose_pivot);

    assert(util::sequences_are_equal(actual, sorted));
}


int main(int argc, char** argv) {
    srand(time(NULL));

    std::vector<int> rand_seq(10000);
    std::generate_n(rand_seq.begin(), rand_seq.size(), util::randint(1000));

    std::vector<int> sorted_seq(rand_seq);
    std::sort(sorted_seq.begin(), sorted_seq.end());

    std::vector<std::vector<int> > inputs;
    inputs.push_back(rand_seq);
    inputs.push_back(sorted_seq);

    std::vector<std::vector<int> >::iterator i;
    for (i = inputs.begin(); i != inputs.end(); i++) {
        test_quicksort(*i, quicksort::First());
        test_quicksort(*i, quicksort::Last());
        test_quicksort(*i, quicksort::Random());
        test_quicksort(*i, quicksort::MedianOfThree());
    }

    std::cout << "Tests passed." << std::endl;
    return 0;
}
