#ifndef __HEAP_SORT__
#define __HEAP_SORT__
#include <string>
#include <vector>

namespace CS32 {
class Tracker;

#define LEFT(i)   ((i) << 1) + 1
#define RIGHT(i)  ((i) << 1) + 2
#define PARENT(i) ((i)-1) >> 1

struct Heap {
  enum SortType { MAX_HEAP, MIN_HEAP };

  Heap(std::vector<int> &array, Tracker &tracker, SortType sortType);

  /// @brief compare 2 elements and return the index of larger or smaller based
  /// on the sort type
  /// @param first the index of the first element
  /// @param second the index of the second element
  /// @return the index of the larger/smaller element
  size_t compare(size_t first, size_t second) const {
    // return m_comparator(m_array[first], m_array[second]) ? first : second;
    if (MAX_HEAP == m_type)
      return m_array[first] > m_array[second] ? first : second;
    else
      return m_array[first] < m_array[second] ? first : second;
  };

  /// @brief build and maintain a heap
  /// @param pos the position of an element needs to be rebalanced
  /// @param heapSize the size of the heap
  void heapify(int pos, int heapSize);

  std::vector<int> &m_array;
  Tracker          &m_tracker;
  SortType          m_type;
};

struct HeapSort {
  void        sort(std::vector<int> &array,
                   Tracker          &tracker,
                   Heap::SortType    sortType = Heap::SortType::MAX_HEAP);
  std::string name() const { return "Heap Sort"; }

private:
};
} // namespace CS32
#endif