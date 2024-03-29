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


// Returns an integer in range [min, max) chosen uniformly at random.
//
// Credit goes to Ryan Reich: http://stackoverflow.com/a/6852396
int random_range(unsigned min, unsigned max) {
    int base_random = rand(); // in [0, RAND_MAX]
    if (RAND_MAX == base_random)
        return random_range(min, max);
    // now guaranteed to be in [0, RAND_MAX)
    int range = max - min,
    remainder = RAND_MAX % range,
    bucket = RAND_MAX / range;
    // There are range buckets, plus one smaller interval within remainder
    // of RAND_MAX
    if (base_random < RAND_MAX - remainder)
        return min + base_random/bucket;
    return random_range (min, max);
}


struct randint {
    int start;
    int stop;

    randint(int stop) : start(0), stop(stop) {}
    randint(int start, int stop) : start(start), stop(stop) {}

    int operator()() {
        return random_range(this->start, this->stop);
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
