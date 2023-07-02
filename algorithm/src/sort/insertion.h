#ifndef __INSERTION_SORT__
#define __INSERTION_SORT__

#include "isort.h"

#include <vector>
#include <string>

namespace CS32
{
    class Tracker;

    struct InsertionSort : public ISort
    {
        void sort(std::vector<int> &array, Tracker &tracker) override;
        std::string name() const override { return "Insertion Sort"; }
    };
}

#endif
