#ifndef __MERGE_SORT__
#define __MERGE_SORT__

#include <vector>

namespace CS32
{
  class Tracker;

  struct MergeSort
  {
    void sort(std::vector<int> &array, Tracker &tracker);
  };
}

#endif