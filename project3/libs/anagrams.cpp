#include "anagrams.h"

#include <fstream>
#include <string>

using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

int vocabularyCreator(istream &dictfile, string dict[]) {
  int    num = 0;
  string word;

  while (getline(dictfile, word) && num < MAXDICTWORDS) {
    dict[num++] = word;
  }

  int result = num;
  while (num < MAXDICTWORDS) {
    // clear the rest of the array if the words in the dictfile < MAXDICTWORDS
    dict[num++] = "";
  }
  return result;
}

int potentialSequences(std::string       word,
                       const std::string dict[],
                       int               size,
                       std::string       results[]) {
  return 0;
}

void outcomeDisclosure(const std::string results[], int size) {}
