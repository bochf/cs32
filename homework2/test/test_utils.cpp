#include "test_utils.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using namespace ::testing;

void validateEightClap(const string &origin, const string &result) {
  size_t pos = 1;
  while (pos < result.length()) {
    if (result[pos] == '8') {
      // if it is '8' then must be 2 '8'
      EXPECT_EQ('8', result[pos + 1]);
      // adjacencies of "88" should be the same
      EXPECT_EQ(result[pos - 1], result[pos + 2]);
      pos += 3;
    } else {
      // if it is not '8', then it must not be the same with the prior one
      EXPECT_NE(result[pos], result[pos - 1]);
      ++pos;
    }
  }

  string copy;
  auto   it =
      copy_if(result.begin(), result.end(), copy.begin(), [](unsigned char c) {
        return c != '8';
      });
  copy.erase(it, copy.end());
  // the result removes all '8' should be the same as origin
  EXPECT_EQ(origin, copy);
}

void validateConeHeads(const std::string &origin, const std::string &result) {
  size_t start = origin.find('<');
  if (string::npos == start) {
    EXPECT_EQ("", result);
    return;
  }

  size_t end = origin.find('>', start);
  if (string::npos == end) {
    EXPECT_EQ("", result);
    return;
  }

  string expectedResult = origin.substr(start, end + 1 - start);
  EXPECT_EQ(expectedResult, result);
}

void getAllSum(set<int>          &possible,
               set<int>          &impossible,
               const vector<int> &input) {
  assert(input.size() <= 32); // support up to 32 elements of the input list
  possible.clear();
  impossible.clear();

  for (size_t mask = 0; mask < (1 << input.size()); ++mask) {
    int sum = 0;
    for (size_t j = 0; j < input.size(); ++j) {
      if (1 << j & mask) {
        sum += input[j];
      }
    }
    possible.insert(sum);
  }

  // add values out of possible range
  impossible.insert(*possible.begin() - 1);
  impossible.insert(*possible.rbegin() + 1);

  int prev = *impossible.begin();
  for (int x : possible) {
    if (x - prev > 1) {
      int value = (x + prev) / 2;
      impossible.insert(value);
    }
    prev = x;
  }
}
