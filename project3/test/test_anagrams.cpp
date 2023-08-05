#include "anagrams.h"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace ::testing;
using namespace std;

TEST(TestReadFile, testReadNormalFile) {
  string dict[MAXDICTWORDS];

  ifstream infile("words.txt");
  int      num = vocabularyCreator(infile, dict);
  EXPECT_EQ(num, 25144); // the file has 25144 lines
}

TEST(TestReadFile, testReadEmpty) {
  string dict[MAXDICTWORDS];

  ifstream infile("empty.txt");
  int      num = vocabularyCreator(infile, dict);
  EXPECT_EQ(num, 0); // the file has no word
}

TEST(TestReadFile, testReadLargeFile) {
  string dict[MAXDICTWORDS];

  ifstream infile("exceed.txt");
  int      num = vocabularyCreator(infile, dict);
  EXPECT_EQ(num, MAXDICTWORDS); // the file contains more words than the maximum
}

class TestOutput : public Test {
protected:
  streambuf   *originBuffer;
  stringstream coutBuffer;

  void SetUp() override {
    // redirect standard output to a string buffer for result verification
    originBuffer = cout.rdbuf();
    cout.rdbuf(coutBuffer.rdbuf());
  }

  void TearDown() override {
    // restore the standard output
    cout.rdbuf(originBuffer);
  }
};

TEST_F(TestOutput, testPrint) {
  string results[] = {"a", "b", "c", "d"};
  outcomeDisclosure(results, 4);

  EXPECT_EQ("a\nb\nc\nd\n", coutBuffer.str());
}

TEST_F(TestOutput, testEmptyResults) {
  string results[] = {};
  outcomeDisclosure(results, 0);

  EXPECT_EQ("", coutBuffer.str());
}

class TestAnagram : public Test {
protected:
  string dict[MAXDICTWORDS];
  string results[MAXRESULTS];
  int    dictSize    = 0;
  int    resultCount = 0;

  void clear() {
    for (int i = 0; i < MAXDICTWORDS; ++i) {
      dict[i]  = "";
      dictSize = 0;
    }
    for (int i = 0; i < MAXRESULTS; ++i) {
      results[i]  = "";
      resultCount = 0;
    }
  }

  void SetUp() override { clear(); }

  void loadDict(const string &filename) {
    ifstream infile(filename);
    dictSize = vocabularyCreator(infile, dict);
  }

  bool anagram(const string &str1, const string &str2) {
    // compare 2 strings are anagrams
    string tmp1(str1);
    string tmp2(str2);
    sort(tmp1.begin(), tmp1.end());
    sort(tmp2.begin(), tmp2.end());

    return tmp1 == tmp2;
  }

  bool isUnique() {
    // check each string int he list is unique
    if (resultCount < 2) {
      return true;
    }

    for (size_t i = 0; i < resultCount - 1; ++i) {
      for (size_t j = i + 1; j < resultCount; ++j) {
        if (results[i] == results[j]) {
          return false;
        }
      }
    }

    return true;
  }
};

TEST_F(TestAnagram, testPermute) {
  loadDict("words.txt");
  int count = potentialSequences("tea", dict, dictSize, results);
  EXPECT_EQ(4, count);
  EXPECT_TRUE(isUnique());
  for (size_t i = 0; i < 4; ++i) {
    EXPECT_TRUE(anagram("tea", results[i]));
  }
}

TEST_F(TestAnagram, testMoreThanResults) {
  loadDict("makeup.txt");
  int count = potentialSequences("abcd", dict, dictSize, results);
  EXPECT_EQ(MAXRESULTS, count);
  EXPECT_TRUE(isUnique());
  for (size_t i = 0; i < MAXRESULTS; ++i) {
    EXPECT_TRUE(anagram("abcd", results[i]));
  }
}

TEST_F(TestAnagram, testDuplicate) {
  loadDict("makeup.txt");
  int count = potentialSequences("abb", dict, dictSize, results);
  EXPECT_EQ(3, count);
  EXPECT_TRUE(isUnique());
  for (size_t i = 0; i < 3; ++i) {
    EXPECT_TRUE(anagram("abb", results[i]));
  }
}