#include <algorithm>
#include <time.h>

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