#ifndef ALGORITHMS_UTIL_H
#define ALGORITHMS_UTIL_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace algorithms {
namespace util {

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
    int start;
    int stop;
public:
    randint(int stop) : start(0), stop(stop) {}
    randint(int start, int stop) : start(start), stop(stop) {}

    int operator()() {
        return rand() % (this->stop - 1) + start;
    }
};


// trim from start
static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}


// trim from end
static inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
            s.end());
    return s;
}


// trim from both ends
static inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}


std::vector<std::string>& split(const std::string& s,
                                char delim,
                                std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


} // namespace util
} // namespace algorithms

#endif // ALGORITHMS_UTIL_H
