#pragma once
#include <vector>

template <typename T>
class SortBase
{
public:
    /**
     * \brief Swaps two values
     * \param left The left value
     * \param right The right value
     */
    static void swap(T &left, T &right)
    {
        T temp = left;
        left = right;
        right = temp;
    }

    static void swap(std::vector<T> &arr, int a, int b)
    {
        T temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
};