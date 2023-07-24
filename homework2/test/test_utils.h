#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#include <iostream>
#include <set>
#include <string>
#include <vector>

/// @brief validate a string is correctly converted by eightClap
/// a string where the same characters next each other are separated by "88"
/// (assumeing the input string will not have 8’s)
/// Example:
///  eightClap("goodbye") => "go88odbye"
///  eightClap("yyuu") => "y88yu88u"
///  eightClap("aaaa") => "a88a88a88a"
/// @param origin
/// @param result
void validateEightClap(const std::string &origin, const std::string &result);

/// @brief validate the sub string between '<' and '>' is extracted from the
/// original string ('<' and '>' is included)
/// Example :
///  coneHeads("abc<ghj>789") => "<ghj>"
///  coneHeads("<x>7") => "<x>"
///  coneHeads("4agh<y>") => "<y>"
///  coneHeads("4agh<>") => "<>"
/// @param origin
/// @param result
void validateConeHeads(const std::string &origin, const std::string &result);

/// @brief get sums of all combinations of the given list of values and generate
/// some of impossible sums
/// @param possible stores possible sums
/// @param impossible stores impossible sums
/// @param a input list
/// @param len number of element in the array
void getAllSum(std::set<int>          &possible,
               std::set<int>          &impossible,
               const std::vector<int> &input);

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &array) {
  os << "[";
  for (const auto &value : array) {
    os << value << " ";
  }
  os << "]";

  return os;
};

#endif