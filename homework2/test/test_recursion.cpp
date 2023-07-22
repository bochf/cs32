#include "recursion.h"

#include <algorithm>
#include <string>

#include <gtest/gtest.h>

using namespace std;
using namespace ::testing;

TEST(TestRecursion, product) {
  // list of m, n and the expected result of m * n
  unsigned int testParams[][2] = {
      {0,    0   },
      {1,    0   },
      {0,    1   },
      {2,    2   },
      {1234, 5678}
  };

  for (const auto operands : testParams) {
    unsigned int m              = operands[0];
    unsigned int n              = operands[1];
    int          expectedResult = m * n;
    EXPECT_EQ(expectedResult, productOfTheSystem(m, n));
  }
}

TEST(TestRecursion, accountForADigit) {
  // list of parameters and expected result
  int testParams[][3] = {
      {-18838, 8, 3},
      {55555,  3, 0},
      {0,      0, 0},
      {-18838, 8, 3},
  };

  for (const auto operands : testParams) {
    int num            = operands[0];
    int digit          = operands[1];
    int expectedResult = operands[2];
    EXPECT_EQ(expectedResult, accountForADigit(num, digit));
  }
}

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

TEST(TestRecursion, eightClap) {
  string testParams[] = {"goodbye", "yyuu", "aaaa"};

  for (const auto &origin : testParams) {
    string actualResult = eightClap(origin);
    validateEightClap(origin, actualResult);
  }
}

TEST(TestRecursion, coneHeads) {}

TEST(TestRecursion, conglomerateOfNumbers) {}

TEST(TestRecursion, findAWay) {}
