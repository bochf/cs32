#ifndef __INSERTION_SORT__
#define __INSERTION_SORT__

#include <vector>

namespace CS32
{
    class Tracker;

    struct InsertionSort
    {
        void sort(std::vector<int> &array, Tracker &tracker);
    };
}

#endif
