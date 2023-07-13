#include "merge.h"
#include "tracker.h"

#include <vector>

using namespace std;

namespace CS32
{
  /**
   * @brief Merge 2 sorted array in place
   *
   * @param array, the array to be sorted
   * @param left, the index of the first element of the first array
   * @param mid, the index of the last element of the first array, and mid+1 is the first element of the second array
   * @param right, the index of the last element of the second array
   */
void merge(int array[], int left, int mid, int right, Tracker &tracker) {
  vector<int> tmp;         // a temp array to store the sort result
  int         i = left;    // pointing to the current element of the first array
  int         j = mid + 1; // point to the current element of the second array

  while (i <= mid && j <= right) { // compare elements from 2 arrays, store the
                                   // less one to temp array
    if (array[i] <= array[j]) {
      tmp.push_back(array[i++]);
    } else {
      tmp.push_back(array[j++]);
    }
    tracker.increaseStep();
  }

  // copy the rest of the elements of the first array to the temp array
  while (i <= mid) {
    tmp.push_back(array[i++]);
    tracker.increaseStep();
  }

  // copy the temp array to the original array
  for (const auto &value : tmp) {
    array[left++] = value;
    tracker.increaseStep();
  }
}

void mergeSort(int array[], int left, int right, Tracker &tracker) {
  tracker.increaseStep();

  if (left >= right) { // less than 2 elements, the array is sorted
    return;
  }

  int mid = (left + right) >> 1;
  mergeSort(array, left, mid, tracker);
  mergeSort(array, mid + 1, right, tracker);
  merge(array, left, mid, right, tracker);
}

void MergeSort::sort(vector<int> &array, Tracker &tracker) const {
  mergeSort(array.data(), 0, array.size() - 1, tracker);
}
}