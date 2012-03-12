from __future__ import division


def mergesort(seq):
    n = len(seq)

    if n < 2:
        return seq

    left = seq[0:n//2]
    right = seq[n//2:]
    return merge(mergesort(left), mergesort(right))


def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    # When we reach the end of either subsequence, copy the remaining portion
    # to the result. Since both subsequences are already in sorted order, we do
    # not have to reorder the remaining portion.
    result.extend(left[i:] + right[j:])

    return result


def test_mergesort():
    import random
    for _ in range(1000):
        seq = [random.randint(0, 99) for i in range(1000)]
        assert(mergesort(seq[:]) == list(sorted(seq)))


if __name__ == '__main__':
    test_mergesort()
