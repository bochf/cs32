#ifndef __MERGE_SORT__
#define __MERGE_SORT__

#include "isort.h"

#include <vector>
#include <string>

namespace CS32
{
  class Tracker;

  struct MergeSort : public ISort
  {
    void sort(std::vector<int> &array, Tracker &tracker) override;
    std::string name() const override { return "Merge Sort"; }
  };
}

#endif