# coding: utf-8
'''Counting array inversions'''

from __future__ import division
from __future__ import print_function


def merge_and_count_inversions(left, right):
    sorted_arr = []
    cross_inversions = 0
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            sorted_arr.append(left[i])
            i += 1
        else:
            sorted_arr.append(right[j])
            cross_inversions += len(left) - i
            j += 1

    sorted_arr.extend(left[i:] + right[j:])
    return sorted_arr, cross_inversions


def sort_and_count_inversions(arr):
    '''Performs a merge-sort and counts the number of inversions during each
    merge step.'''
    n = len(arr)

    if n < 2:
        return (arr, 0)

    middle = n // 2
    sorted_left, left_inversions = sort_and_count_inversions(arr[:middle])
    sorted_right, right_inversions = sort_and_count_inversions(arr[middle:])
    sorted_arr, cross_inversions = merge_and_count_inversions(sorted_left,
                                                              sorted_right)
    return (sorted_arr, left_inversions + right_inversions + cross_inversions)


def test_count_inversions():
    test_inputs = [
        ([], 0),
        ([5], 0),
        ([5, 4], 1),
        ([4, 4], 0),
        ([4, 5], 0),
        ([2, 4, 1, 3, 5], 3),
        ([1, 2, 3, 4, 5, 6], 0),
        ([6, 5, 4, 3, 2, 1], 15),
    ]

    for arr, expected_inversions in test_inputs:
        sorted_arr, inversions = sort_and_count_inversions(arr)
        assert(sorted_arr == list(sorted(arr)))
        assert(inversions == expected_inversions)


if __name__ == '__main__':
    test_count_inversions()

    with open('IntegerArray.txt') as random_ints:
        arr = [int(line) for line in random_ints]
        sorted_arr, inversions = sort_and_count_inversions(arr)
        assert(sorted_arr == list(sorted(sorted_arr)))
        assert(inversions == 2407905288)
        print('Tests passed.')
