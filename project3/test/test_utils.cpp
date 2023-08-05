#include "anagrams.h"
#include "utils.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace ::testing;
using namespace std;

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

TEST(TestSearch, testSearch) {
  string dict[10] = {"apple",
                     "banana",
                     "carrot",
                     "date",
                     "elderburry",
                     "fig",
                     "grape",
                     "honeydew melon",
                     "kiwi",
                     "lemon"};
  EXPECT_TRUE(searchWord("apple", dict, 10));
  EXPECT_TRUE(searchWord("date", dict, 10));
  EXPECT_FALSE(searchWord("peach", dict, 10));
}

TEST(TestSearch, testLargeDict) {
  string dict[MAXDICTWORDS];

  ifstream infile("words.txt");
  int      num = vocabularyCreator(infile, dict);

  for (int index = 0; index < 100; ++index) {
    EXPECT_TRUE(searchWord(dict[index], dict, num, 101));
  }

  EXPECT_FALSE(searchWord("xyz", dict, num, 101));
}

TEST(TestPermut, testPermut) {
  struct SearchAndSave {
    string m_result[MAXRESULTS];
    string m_dict[MAXDICTWORDS];
    int    m_resultsNum = 0;
    int    m_dictNum    = 0;

    explicit SearchAndSave(const string &filename) {
      ifstream infile(filename);
      m_dictNum = vocabularyCreator(infile, m_dict);
    }

    int execute(const string &str) {
      if ((m_resultsNum < MAXRESULTS)                 // not exceed max results
          && !searchWord(str, m_result, m_resultsNum) // not already saved
          && searchWord(str, m_dict, m_dictNum)       // in the dictionary
      ) {
        m_result[m_resultsNum++] = str;
      }

      return m_resultsNum;
    }
  };

  SearchAndSave searchAndSave("words.txt");
  auto          callable = [&searchAndSave](const string &str) {
    return searchAndSave.execute(str);
  };
  string word("tea");
  permutate(callable, word, 0);

  EXPECT_TRUE(searchWord("tea", searchAndSave.m_result, MAXRESULTS));
  EXPECT_TRUE(searchWord("eta", searchAndSave.m_result, MAXRESULTS));
  EXPECT_TRUE(searchWord("eat", searchAndSave.m_result, MAXRESULTS));
  EXPECT_TRUE(searchWord("ate", searchAndSave.m_result, MAXRESULTS));
}