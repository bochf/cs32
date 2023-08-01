#include "anagrams.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace ::testing;
using namespace std;

TEST(TestReadFile, testReadNormalFile) {
  string dict[MAXDICTWORDS];

  ifstream infile("words.txt");
  int      num = vocabularyCreator(infile, dict);
  outcomeDisclosure(dict, 10);
  EXPECT_EQ(num, 25144); // the file has 25144 lines
}

TEST(TestReadFile, testReadEmpty) {
  string dict[MAXDICTWORDS];

  ifstream infile("empty.txt");
  int      num = vocabularyCreator(infile, dict);
  outcomeDisclosure(dict, 10);
  EXPECT_EQ(num, 0); // the file has no word
}

TEST(TestReadFile, testReadLargeFile) {
  string dict[MAXDICTWORDS];

  ifstream infile("exceed.txt");
  int      num = vocabularyCreator(infile, dict);
  outcomeDisclosure(dict, 10);
  EXPECT_EQ(num, MAXDICTWORDS); // the file contains more words than the maximum
}