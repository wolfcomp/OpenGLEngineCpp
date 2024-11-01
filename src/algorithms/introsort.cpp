#include "introsort.h"
#include <cmath>
#include <glm/vec3.hpp>

void IntroSort<int>::heap_sort(std::vector<int> &arr)
{
    int count = static_cast<int>(arr.size());
    heapify(arr, count--);
    while (count > 0)
    {
        swap(arr, 0, count--);
        shift_down(arr, 0, count);
    }
}

void IntroSort<int>::heapify(std::vector<int> &arr, int right)
{
    int start = (right - 2) / 2;
    while (start >= 0)
    {
        shift_down(arr, start, right - 1);
        start--;
    }
}

void IntroSort<int>::shift_down(std::vector<int> &arr, int left, int right)
{
    int i = leaf_search(arr, left, right);
    while (arr[left] > arr[i])
        i = (i - 1) / 2;
    int temp = arr[i];
    arr[i] = arr[left];
    while (i > left)
    {
        int p = (i - 1) / 2;
        swap(temp, arr[p]);
        i = p;
    }
}

int IntroSort<int>::leaf_search(std::vector<int> arr, int left, int right)
{
    int i = left;
    while (2 * i + 2 <= right)
    {
        if (arr[2 * i + 2] > arr[2 * i + 1])
            i = 2 * i + 2;
        else
            i = 2 * i + 1;
    }
    if (2 * i + 1 <= right)
        i = 2 * i + 1;
    return i;
}

void IntroSort<int>::insertion_sort(std::vector<int> &arr)
{
    int i = 1;
    while (i < arr.size())
    {
        int j = i;
        while (j > 0 && compare(arr[j - 1], arr[j]) == 1)
        {
            swap(arr, j, j - 1);
            j--;
        }
        i++;
    }
}

int IntroSort<int>::partition(std::vector<int> &arr, int left, int right)
{
    const int pivot = arr[(right - left) / 2 + left];
    int i = left - 1, j = right + 1;
    while (true)
    {
        do
            i++;
        while (arr[i] < pivot);
        do
            j--;
        while (arr[j] > pivot);
        if (i >= j)
            return j;
        swap(arr, i, j);
    }
}

std::vector<int> IntroSort<int>::merge(std::vector<int> &lower, std::vector<int> &upper)
{
    std::vector<int> ret = std::vector<int>();
    for (auto i : lower)
        ret.push_back(i);
    for (auto i : upper)
        ret.push_back(i);
    return ret;
}

int IntroSort<int>::compare(int l, int r)
{
    if (l < r)
        return -1;
    if (l == r)
        return 0;
    return 1;
}