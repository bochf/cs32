#ifndef __UTILS__
#define __UTILS__

#include <string>

namespace CS32
{
  /// @brief make a large number present in more human readable way
  /// @param num the number
  /// @return number scaled in K, M, G
  std::string humanize(uint64_t num);
}
#endif