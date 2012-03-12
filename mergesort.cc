#include <algorithm>
#include <cassert>
#include <time.h>
#include <vector>


template <typename Container>
Container merge(const Container& left,
                const Container& right) {
    Container result;

    typename Container::const_iterator i, j;
    i = left.begin();
    j = right.begin();

    while (i != left.end() && j != right.end()) {
        if (*i <= *j) {
            result.push_back(*i);
            i++;
        }
        else {
            result.push_back(*j);
            j++;
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


template <typename Container1, typename Container2>
bool sequences_are_equal(const Container1& seq1, const Container2& seq2) {
    typedef typename Container1::const_iterator Iter1;
    typedef typename Container2::const_iterator Iter2;
    typedef std::pair<Iter1, Iter2> IterPair;
    IterPair mismatch_pair = std::mismatch(seq1.begin(), seq1.end(),
                                           seq2.begin());
    return mismatch_pair.first == seq1.end();
}


struct randint {
    int range;
public:
    randint(int range) : range(range) {}

    int operator()() {
        return (rand() / RAND_MAX) * range;
    }
};


void test_mergesort() {
    typedef std::vector<int> Vector;
    Vector seq(1000);
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        std::generate_n(seq.begin(), seq.size(), randint(100));
        Vector sorted(seq.begin(), seq.end());
        std::sort(sorted.begin(), sorted.end());
        assert(sequences_are_equal(mergesort(seq), sorted));
    }
}


int main(int argc, char** argv) {
    test_mergesort();
}
