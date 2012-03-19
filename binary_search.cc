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
