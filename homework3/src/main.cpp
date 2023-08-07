#include "sorts.cpp"

#include <iostream>
#include <vector>

using namespace std;

int main() {
  size_t nsensors;
  cout << "Enter number of sensors to sort: ";
  cin >> nsensors;
  if (!cin || nsensors <= 0) {
    cout << "You must enter a positive number.  Goodbye." << endl;
    return 1;
  }

  // Create a random ordering of id numbers 0 through nsensors-1
  vector<IdType> ids;
  for (size_t j = 0; j < nsensors; j++)
    ids.push_back(IdType(j));
  // The following are three options:
  // OPTION 1: good for C++ 14 or earlier - if using Visual Studio Community or
  // Enterprise, this should work
  //   random_shuffle(ids.begin(), ids.end()); // from <algorithm>

  // OPTION 2:  an alternative if compiling in C++ 17 or later
  
  std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(ids.begin(), ids.end(), g);
  

  // Create a bunch of Sensors
  vector<Sensor> unorderedSensors;
  for (size_t k = 0; k < ids.size(); k++)
    unorderedSensors.push_back(Sensor(ids[k]));

  // Create a timer

  Timer timer;

  // Sort the Sensors using the STL sort algorithm.  It uses a variant
  // of quicksort called introsort.

  vector<Sensor> sensors(unorderedSensors);
  timer.start();
  sort(sensors.begin(), sensors.end(), compareSensor);
  double elapsed = timer.elapsed();
  assert(isSorted(sensors));
  report("STL sort", elapsed, sensors);

  // Sort the already sorted array using the STL sort.  This should be
  // fast.

  timer.start();
  sort(sensors.begin(), sensors.end(), compareSensor);
  elapsed = timer.elapsed();
  assert(isSorted(sensors));
  report("STL sort if already sorted", elapsed, sensors);

  if (TEST_INSERTION_SORT) {
    // Sort the original unsorted array using insertion sort.  This
    // should be really slow.  If you have to wait more than a minute,
    // try rerunning the program with a smaller number of Sensors.

    sensors = unorderedSensors;
    timer.start();
    insertion_sort(sensors, compareSensor);
    elapsed = timer.elapsed();
    assert(isSorted(sensors));
    report("insertion sort if not already sorted", elapsed, sensors);

    // Sort the already sorted array using insertion sort.  This should
    // be fast.

    timer.start();
    insertion_sort(sensors, compareSensor);
    elapsed = timer.elapsed();
    assert(isSorted(sensors));
    report("insertion sort if already sorted", elapsed, sensors);
  }

  // Since Sensors are expensive to copy, and since the STL's sort copies
  // Sensors O(N log N) times, let's sort POINTERS to the Sensors, then
  // make one final pass to rearrange the Sensors according to the
  // reordered pointers.  We'll write some code; you write the rest.

  // Set sensors to the original unsorted sequence
  sensors = unorderedSensors;

  // Start the timing
  timer.start();

  // Create an auxiliary copy of sensors, to faciliate the later reordering.
  // We create it in a local scope so that we also account for the
  // destruction time.
  {
    vector<Sensor> auxSensors(sensors);

    // TODO:  Create a vector of Sensor pointers, and set each pointer
    //        to point to the corresponding Sensor in auxSensors.

    // TODO:  Sort the vector of pointers using the STL sort algorithm
    //        with compareSensorPtr as the ordering relationship.

    // TODO:  Using the now-sorted vector of pointers, replace each Sensor
    //        in sensors with the Sensors from auxSensors in the correct order.

  } // auxSensors will be destroyed here

  // Report the timing and verify that the sort worked
  elapsed = timer.elapsed();
  assert(isSorted(sensors));
  report("STL sort of pointers", elapsed, sensors);

  return 0;
}
