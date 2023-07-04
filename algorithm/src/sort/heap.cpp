#include "heap.h"
#include "tracker.h"
#include "utils.h"

#include <iostream>
#include <vector>

using namespace std;

namespace CS32 {

///
/// Class Heap
///
Heap::Heap(std::vector<int> &array, Tracker &tracker, SortType sortType)
    : m_array(array), m_tracker(tracker), m_type(sortType) {}

void Heap::heapify(int pos, int heapSize) {
  m_tracker.increaseStep();
  int tmp  = pos;
  int left = LEFT(pos);

  if (left < heapSize)
    tmp = compare(pos, left);
  else
    return;

  int right = RIGHT(pos);
  if (right < heapSize)
    tmp = compare(tmp, right);

  if (tmp != pos) {
    swap(m_array, tmp, pos);
    heapify(tmp, heapSize);
  }
}

bool validHeap(const vector<int> &array) {
  size_t len      = array.size();
  size_t heapSize = len + 1 / 2;
  for (int parent = 0; parent < heapSize; ++parent) {
    int left  = LEFT(parent);
    int right = RIGHT(parent);
    if (left < len && array[parent] < array[left]) {
      cout << "Invalid heap, heap[" << parent << "]=" << array[parent]
           << ", heap[" << left << "]=" << array[left] << endl;
      return false;
    }
    if (right < len && array[parent] < array[right]) {
      cout << "Invalid heap, heap[" << parent << "]=" << array[parent]
           << ", heap[" << right << "]=" << array[right] << endl;
      return false;
    }
  }

  return true;
}

///
/// Class HeapSort
///

void HeapSort::sort(vector<int>   &array,
                    Tracker       &tracker,
                    Heap::SortType sortType) {
  tracker.increaseStep();

  size_t len = array.size();
  if (len < 1)
    return;

  // construct the heap
  Heap   heap(array, tracker, sortType);
  size_t pos = (len + 1) >> 1;
  do {
    --pos;
    heap.heapify(pos, len);
  } while (pos > 0);

  // sort the array based on heap
  for (int i = len - 1; i >= 0; --i) {
    swap(array, i, 0);
    heap.heapify(0, i);
  }
}

} // namespace CS32