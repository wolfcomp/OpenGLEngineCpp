#pragma once
#include "sortbase.h"

/**
 * \brief Implements all functions needed for IntroSort
 */
template <typename T>
class IntroSort : public SortBase<T>
{
    /**
     * \brief Partitions the array into two arrays
     * \param arr The array to partition
     * \param left The left index
     * \param right The right index
     * \return The pivot index of the array (based on right index)
     */
    static int partition(std::vector<T> &arr, int left, int right);

    /**
     * \brief Sorts the array using insertion sort
     * \param arr The array to sort
     */
    static void insertion_sort(std::vector<T> &arr);

    /**
     * \brief Sorts the array using heap sort
     * \param arr The array to sort
     */
    static void heap_sort(std::vector<T> &arr);

    /**
     * \brief Creates a heap from the array
     * \param arr The array to create a heap from
     * \param right The index to start from
     */
    static void heapify(std::vector<T> &arr, int right);

    /**
     * \brief Sorts the array using best of heap sort, insertion sort and quick sort based on the depth/size
     * \param arr The array to sort
     * \param depth The depth of the array (2 * logarithm of the array size)
     */
    static void sort(std::vector<T> &arr, int depth)
    {
        if (arr.size() < 16)
        {
            return insertion_sort(arr);
        }
        if (depth == 0)
        {
            return heap_sort(arr);
        }
        const int p = partition(arr, 0, arr.size() - 1);
        auto lower = std::vector<T>(arr.begin(), arr.begin() + p);
        auto upper = std::vector<T>(arr.begin() + p, arr.end());
        sort(lower, depth - 1);
        sort(upper, depth - 1);
        arr = merge(lower, upper);
    }

    /**
     * \brief Searches for the first leaf in the array
     * \param arr The array to search in
     * \param left The left index
     * \param right The right index
     * \return The index of the first leaf
     */
    static int leaf_search(std::vector<T> arr, int left, int right);

    /**
     * \brief Shifts the array down
     * \param arr The array to shift
     * \param left The left index
     * \param right The right index
     */
    static void shift_down(std::vector<T> &arr, int left, int right);

    /**
     * \brief Merges two arrays into one
     * \param lower The lower array
     * \param upper The upper array
     * \return The merged array
     */
    static std::vector<int> merge(std::vector<T> &lower, std::vector<T> &upper);

    /**
     * \breif How to sort the two values
     * \param l The left value
     * \param r The right value
     * \return -1 if l < r, 0 if l == r, 1 if l > r
     */
    static int compare(T l, T r);

public:
    /**
     * \brief Sorts the array using IntroSort
     * \param arr The array to sort
     */
    static void sort(std::vector<T> &arr)
    {
        sort(arr, 2 * static_cast<int>(log(arr.size())));
    }
};