#include "anagrams.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int fillDict(istream &dictfile, string dict[], size_t maxSize) {
  if (dictfile.eof() || maxSize == 0) {
    return 0;
  }

  if (getline(dictfile, dict[0])) {
    return fillDict(dictfile, &dict[1], maxSize - 1) + 1;
  }

  return 0;
}

int vocabularyCreator(istream &dictfile, string dict[]) {
  /// There is a potential stack space issue since we go through the file all
  /// the way to the end.
  /// @note the words stored in the `dict` is in reverse order

  if (string word; getline(dictfile, word)) {
    int num = vocabularyCreator(dictfile, dict);
    if (num < MAXDICTWORDS) {
      dict[num] = word;
      return num + 1;
    }

    return num;
  }
  return 0;

  // return fillDict(dictfile, dict, MAXDICTWORDS);
}

int potentialSequences(string       word,
                       const string dict[],
                       int          size,
                       string       results[]) {
  return 0;
}

void outcomeDisclosure(const string results[], int size) {
  if (size < 1) {
    cerr << endl;
    return;
  }

  cerr << results[0] << " ";
  outcomeDisclosure(&results[1], size - 1);
}