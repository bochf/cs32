#include "quick.h"
#include "tracker.h"

#include <iostream>
#include <vector>

using namespace std;

namespace CS32 {
void quickSort(int array[], int left, int right, Tracker &tracker) {
  tracker.increaseStep();
  if (left < right) {
    int pivot = partition(array, left, right, tracker);
    quickSort(array, left, pivot - 1, tracker);
    quickSort(array, pivot + 1, right, tracker);
  }
}

int partition(int array[], int left, int right, Tracker &tracker) {
  int pivot = array[right];

  int i = left;
  for (int j = left; j < right; ++j) {
    tracker.increaseStep();
    if (array[j] <= pivot) {
      if (i < j) {
        swap(array[i], array[j]);
      }
      ++i;
    }
  }

  if (i < right)
    swap(array[i], array[right]);
  return i;
}

void QuickSort::sort(std::vector<int> &array, Tracker &tracker) {
  if (array.size() < 1)
    return;
  quickSort(array.data(), 0, array.size() - 1, tracker);
}

} // namespace CS32