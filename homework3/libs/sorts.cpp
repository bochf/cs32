#include <algorithm>
#include <cassert>
#include <cstdlib> // for std::rand
#include <iostream>
#include <numeric> // for std::accumulate
#include <random>
#include <string>
#include <vector>

using namespace std;

//========================================================================

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#if __cplusplus >= 201103L // C++11

#include <chrono>

class Timer {
public:
  Timer() { start(); }
  void   start() { m_time = std::chrono::high_resolution_clock::now(); }
  double elapsed() const {
    std::chrono::duration<double, std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
    return diff.count();
  }

private:
  std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER) // not C++11, but Windows

#include <windows.h>

class Timer {
public:
  Timer() {
    QueryPerformanceFrequency(&ticksPerSecond);
    start();
  }
  void   start() { QueryPerformanceCounter(&m_time); }
  double elapsed() const {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (1000.0 * (now.QuadPart - m_time.QuadPart)) /
           ticksPerSecond.QuadPart;
  }

private:
  LARGE_INTEGER m_time;
  LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer {
public:
  Timer() { start(); }
  void   start() { m_time = std::clock(); }
  double elapsed() const {
    return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
  }

private:
  std::clock_t m_time;
};

#endif

//========================================================================

// Here's a class that is not cheap to copy because the objects contain
// a large array.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

const int NREADINGS = 150;

struct Sensor {
  IdType id;
  double readings[NREADINGS];
  double avg;
  Sensor(IdType i) : id(i) {
    // create random sensor readings (from 0 to 99)
    for (size_t k = 0; k < NREADINGS; k++)
      readings[k] = rand() % 100;
    // (accumulate computes 0.0 + readings[0] + readings[1] + ...)
    avg = accumulate(readings, readings + NREADINGS, 0.0) / NREADINGS;
  }
};

inline bool compareSensor(const Sensor &lhs, const Sensor &rhs) {
  // The Sensor with the higher average should come first.  If they have
  // the same average, then the Sensor with the smaller id number should
  // come first.  Return true iff lhs should come first.  Notice that
  // this means that a false return means EITHER that rhs should come
  // first, or there's a tie, so we don't care which comes first,

  if (lhs.avg > rhs.avg)
    return true;
  if (lhs.avg < rhs.avg)
    return false;
  return lhs.id < rhs.id;
}

inline bool compareSensorPtr(const Sensor *lhs, const Sensor *rhs) {
  // TODO: You implement this.  Using the same criteria as compareSensor,
  //       compare two Sensors POINTED TO by lhs and rhs.  Think about
  //       how you can do it in one line by calling compareSensor.
  assert(lhs);
  assert(rhs);

  // Just so this will compile, we'll pretend every comparison results in
  // a tie, so there's no preferred ordering.
  return compareSensor(*lhs, *rhs);
}

bool isSorted(const vector<Sensor> &s) {
  // Return true iff the vector is sorted according to the ordering
  // relationship compareSensor

  for (size_t k = 1; k < s.size(); k++) {
    if (compareSensor(s[k], s[k - 1]))
      return false;
  }
  return true;
}

void insertion_sort(vector<Sensor> &s,
                    bool            comp(const Sensor &, const Sensor &)) {
  // TODO: Using the insertion sort algorithm (pp. 312-313), sort s
  //       according to the ordering relationship passed in as the
  //       parameter comp. There is also a version on the class website.

  // Note:  The insertion sort algorithm on pp. 312-313 of the Carrano
  // book 6th edition is incorrect; someone made a change from the 5th
  // edition and messed it up.  See the errata page entry for page 313 at
  // http://homepage.cs.uri.edu/~carrano/WMcpp6e

  if (s.size() < 2) {
    return;
  }

  for (size_t i = 1; i < s.size(); ++i) {
    auto sensor = s[i];
    // repeatly compare the sensor with each one in the sorted list backforward
    // move the sorted element back if it is greater than the sensor
    size_t j = i;
    while (j > 0 && comp(sensor, s[j - 1])) {
      // if the current sorted elemtent < sensor, move it back
      s[j] = s[j - 1];
      --j;
    }
    s[j] = sensor;
  }
}

// Report the results of a timing test

void report(string caption, double t, const vector<Sensor> &s) {
  cout << t << " milliseconds; size " << s.size() << " " << caption << endl;
}

// Create an auxiliary copy of sensors, to faciliate the later reordering.
// We create it in a local scope so that we also account for the
// destruction time.
void insertion_sort_ptr(vector<Sensor> &sensors) {
  vector<Sensor> auxSensors(sensors);

  // Create a vector of Sensor pointers, and set each pointer
  // to point to the corresponding Sensor in auxSensors.
  vector<Sensor *> p_sensors;
  for (auto &sensor : auxSensors) {
    p_sensors.emplace_back(&sensor);
  }

  // Sort the vector of pointers using the STL sort algorithm
  // with compareSensorPtr as the ordering relationship.
  // sort(p_sensors.begin(), p_sensors.end(), compareSensorPtr);

  if (p_sensors.size() < 2) {
    return;
  }

  for (size_t i = 1; i < p_sensors.size(); ++i) {
    auto sensor = p_sensors[i];
    // repeatly compare the sensor with each one in the sorted list backforward
    // move the sorted element back if it is greater than the sensor
    size_t j = i;
    while (j > 0 && compareSensorPtr(sensor, p_sensors[j - 1])) {
      // if the current sorted elemtent < sensor, move it back
      p_sensors[j] = p_sensors[j - 1];
      --j;
    }
    p_sensors[j] = sensor;
  }
  // Using the now-sorted vector of pointers, replace each Sensor
  // in sensors with the Sensors from auxSensors in the correct
  // order.
  for (size_t i = 0; i < sensors.size(); ++i) {
    sensors[i] = *p_sensors[i];
  }

} // auxSensors will be destroyed here