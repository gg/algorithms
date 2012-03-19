from __future__ import division


def recursive_binary_search(sorted_sequence, key, start=0, end=None):
    '''Returns the index of `key` in the `sorted_sequence`, or None if `key` is
    not in `sorted_sequence`.'''
    end = end or len(sorted_sequence)

    if (end - start) < 0:
        return None

    middle = start + ((end - start) // 2)

    if sorted_sequence[middle] == key:
        return middle
    elif sorted_sequence[middle] < key:
        return recursive_binary_search(sorted_sequence, key, middle + 1, end)
    else:
        return recursive_binary_search(sorted_sequence, key, start, middle - 1)


def iterative_binary_search(sorted_sequence, key):
    '''Returns the index of `key` in the `sorted_sequence`, or None if `key` is
    not in `sorted_sequence`.'''
    start = 0
    end = len(sorted_sequence)

    while start < end:
        middle = start + ((end - start) // 2)
        if sorted_sequence[middle] == key:
            return middle
        elif sorted_sequence[middle] < key:
            start = middle + 1
        else:
            end = middle

    return None


if __name__ == '__main__':
    seq = [1, 1, 2, 5, 9, 11, 11, 11, 12, 18, 29, 37, 38, 40, 67, 78, 94, 94]

    assert(recursive_binary_search(seq, 12) == 8)
    assert(recursive_binary_search(seq, 13) == None)

    assert(iterative_binary_search(seq, 12) == 8)
    assert(iterative_binary_search(seq, 13) == None)
