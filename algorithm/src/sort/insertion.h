#ifndef __INSERTION_SORT__
#define __INSERTION_SORT__

#include <string>
#include <vector>

namespace CS32 {
class Tracker;

struct InsertionSort {
  void        sort(std::vector<int> &array, Tracker &tracker) const;
  std::string name() const { return "Insertion Sort"; }
};
} // namespace CS32

#endif
