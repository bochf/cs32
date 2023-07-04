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

  /// @brief exchange the value of 2 elements in the array
  /// @param array the data
  /// @param first the position of the first element
  /// @param second the position of the second element
  void swap(std::vector<int> &array, size_t first, size_t second);

  /// @brief print the list 
  /// @param data the data
  void printData(const std::vector<int> &data);
  } // namespace CS32
#endif
