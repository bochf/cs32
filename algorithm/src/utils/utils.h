#ifndef __UTILS__
#define __UTILS__

#include <string>
#include <vector>

namespace CS32
{
  /// @brief make a large number present in more human readable way
  /// @param num the number
  /// @return number scaled in K, M, G
  std::string humanize(uint64_t num);

  /// @brief print the list 
  /// @param data the data
  void printData(const std::vector<int> &data);
  } // namespace CS32
#endif
