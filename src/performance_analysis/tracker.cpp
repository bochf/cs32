#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "tracker.h"

using namespace std;
using namespace std::chrono;

namespace CS32
{
  TimeTracker::TimeTracker(const string &algorithmName, int dataSize)
      : m_algorithmName(algorithmName), m_dataSize(dataSize), m_steps(0), m_start(steady_clock::now())
  {
  }

  TimeTracker::~TimeTracker()
  {
    stop();
  }

  void TimeTracker::stop()
  {
    auto end = steady_clock::now();
    duration<double> diff = end - m_start;
    cout.imbue(std::locale("en_US.UTF8"));
    cout << m_algorithmName << " processes data size=" << m_dataSize
         << ", repeats " << m_steps << " times, takes " << setprecision(6)
         << diff.count() * 1000 << " ms." << endl;
  }
}