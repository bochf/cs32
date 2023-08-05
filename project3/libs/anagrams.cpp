#include "anagrams.h"
#include "utils.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

int vocabularyCreator(istream &dictfile, string dict[]) {
  return fillDict(dictfile, dict, MAXDICTWORDS);
}

int potentialSequences(string       word,
                       const string dict[],
                       int          size,
                       string       results[]) {
  int  count     = 0; // number of unique permutations found in the dict
  auto collector = [&results, &dict, &size, &count](const string &str) {
    if ((count < MAXRESULTS)                // not exceed max result
        && searchWord(str, dict, size, 100) // found in the dictionary
        && !searchWord(str, results, count) // not already saved in the results
    ) {
      results[count++] = str;
    }
    return count;
  };

  permutate(collector, word, 0);
  return count;
}

void outcomeDisclosure(const string results[], int size) {
  if (size > 0) {
    cout << results[0] << endl;
    outcomeDisclosure(&results[1], size - 1);
  }
}