#include "recursion.h"
#include "test_utils.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using namespace ::testing;

void printMaze(const string maze[], size_t rows) {
  for (size_t i = 0; i < rows; ++i) {
    cerr << maze[i] << endl;
  }
}

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

TEST(TestRecursion, eightClap) {
  string testParams[] = {"goodbye", "yyuu", "aaaa"};

  for (const auto &origin : testParams) {
    string actualResult = eightClap(origin);
    validateEightClap(origin, actualResult);
  }
}

TEST(TestRecursion, coneHeads) {
  string testParams[] = {
      "abc<ghj>789", "<x>7", "4agh<y>", "4agh<>", "", "<>", "><"};

  for (const auto &origin : testParams) {
    string actualResult = coneHeads(origin);
    validateConeHeads(origin, actualResult);
  }
}

TEST(TestRecursion, conglomerateOfNumbers) {
  vector<vector<int>> scenarios = {
    {2, 4, 8},
    {},
    {-1, -2},
    {-1,  2, 5},
    {0, 0, 0, 0, 0, 0},
    {24, 37, 95, 100, -66, 87, 203}
  };

  for (const auto &input : scenarios) {
    set<int> possible;
    set<int> impossible;
    getAllSum(possible, impossible, input);
    for (const int target : possible) {
      ASSERT_TRUE(conglomerateOfNumbers(input.data(), input.size(), target));
    }
    for (const int target : impossible) {
      ASSERT_FALSE(conglomerateOfNumbers(input.data(), input.size(), target));
    }
  }
}

TEST(TestRecursion, findAWay) {
  string maze[] = {
    "XXXXXXXXXXXXXXXXXXXXXXXX",
    "X......................X",
    "XXXX.XXXXXXXXXXXXXXXXXXX",
    "X......................X",
    "X..........XXXXXXXX....X",
    "X..........X......X....X",
    "X..........X...........X",
    "XXXXXXXXXXXXXXXXXXXXXXXX",
    "X......................X",
    "XXXXXXXXXXXXXXXXXXXXXXXX"
  };

  bool result = findAWay(maze, 10, 24, 8, 22, 1, 22);
  printMaze(maze, 10);
  EXPECT_FALSE(result);
}
