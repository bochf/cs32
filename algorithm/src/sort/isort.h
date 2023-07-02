#ifndef __INTERFACE_SORT__
#define __INTERFACE_SORT__

#include <vector>
#include <string>

namespace CS32
{
  class Tracker;

  struct ISort
  {
    virtual void sort(std::vector<int> &array, Tracker &tracker) = 0;
    virtual std::string name() const = 0;
  };
}

#endif