#ifndef __QUICK_SORT__
#define __QUICK_SORT__

#include <string>
#include <vector>

namespace CS32 {
class Tracker;

void quickSort(int array[], int left, int right, Tracker &tracker);
int  partition(int array[], int left, int right, Tracker &tracker);

struct QuickSort {
  void        sort(std::vector<int> &array, Tracker &tracker);
  std::string name() { return "Quick Sort"; }
};

} // namespace CS32
#endif