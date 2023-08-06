#include "anagrams.h"
#include "utils.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace ::testing;
using namespace std;

int loadFile(string dict[], const string &filename) {
  ifstream infile(filename);
  return vocabularyCreator(infile, dict);
}

TEST(TestRepeat, testLoop) {
  int from = 0;
  int to   = 10;

  vector<int> result;
  auto        collector = [&result](int value) { result.push_back(value); };

  repeat(collector, from, to);

  for (int i = from; i < to; ++i) {
    EXPECT_EQ(result[i], i);
  }
}