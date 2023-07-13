#include "insertion.h"
#include "tracker.h"

#include <vector>

using namespace std;

namespace CS32 {
void InsertionSort::sort(vector<int> &array, Tracker &tracker) const {
  if (array.size() < 2) {
    return;
  }

  for (int startOfUnsort = 1; startOfUnsort < array.size(); ++startOfUnsort) {
    int key = array[startOfUnsort];
    int current = startOfUnsort;
    while (current > 0 && array[current - 1] > key) {
      array[current] = array[current - 1];
      --current;
      tracker.increaseStep();
    }

    array[current] = key;
  }
}
} // namespace CS32
