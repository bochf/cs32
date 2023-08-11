#include "sorts.cpp"

#include <iostream>
#include <vector>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;

size_t getInput() {
  size_t nsensors;
  cout << "Enter number of sensors to sort: ";
  cin >> nsensors;
  if (!cin || nsensors <= 0) {
    cout << "You must enter a positive number.  Goodbye." << endl;
    exit(0);
  }

  return nsensors;
}

void prepareData(vector<Sensor> &sensors, size_t nsensors) {
  // Create a random ordering of id numbers 0 through nsensors-1
  vector<IdType> ids;
  for (size_t j = 0; j < nsensors; j++)
    ids.push_back(IdType(j));

  std::random_device rd;
  std::mt19937       g(rd());
  std::shuffle(ids.begin(), ids.end(), g);

  // Create a bunch of Sensors
  for (size_t k = 0; k < ids.size(); k++)
    sensors.emplace_back(ids[k]);
}

void testSort(const vector<Sensor> &originData,
              size_t                size,
              const string         &name,
              void                  algorithm(vector<Sensor> &)) {
  // copy size of elements from original data to the new vector
  vector<Sensor> sensors(originData.begin(), originData.begin() + size);

  // Start the timing
  Timer timer;
  timer.start();

  // test unordered data sorting
  algorithm(sensors);

  // Report the timing and verify that the sort worked
  report(name + " unordered", timer.elapsed(), sensors);
  assert(isSorted(sensors));

  // restart timer
  timer.start();

  // test sorted data sorting
  algorithm(sensors);

  // Report the timing and verify that the sort worked
  report(name + " sorted", timer.elapsed(), sensors);
  assert(isSorted(sensors));
}

void repeatTestSort(const vector<Sensor> &originData,
                    size_t                start,
                    size_t                increament,
                    size_t                repeat,
                    const string         &name,
                    void                  algorithm(vector<Sensor> &)) {
  for (size_t i = 0; i < repeat; ++i) {
    testSort(originData, start + increament * i, name, algorithm);
  }
}

#define K *1000

int main() {
  // size_t         nsensors = getInput();

  vector<Sensor> unorderedSensors;
  prepareData(unorderedSensors, 1000 K);

  auto stl_sort = [](vector<Sensor> &s) {
    sort(s.begin(), s.end(), compareSensor);
  };
  repeatTestSort(unorderedSensors, 100 K, 20 K, 40, "STL", stl_sort);

  if (TEST_INSERTION_SORT) {
    auto ins_sort = [](vector<Sensor> &s) { insertion_sort(s, compareSensor); };
    repeatTestSort(unorderedSensors, 3 K, 600, 40, "INSERTION", ins_sort);
    repeatTestSort(
        unorderedSensors, 2 K, 400, 40, "INSERTION_PTR", insertion_sort_ptr);
  }

  return 0;
}
