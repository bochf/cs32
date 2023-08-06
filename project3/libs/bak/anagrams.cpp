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
  Collector collector(dict, size, word);
  permutate(collector, word, 0);
  return collector.dumpResult(results);
}

void outcomeDisclosure(const string results[], int size) {
  if (size > 0) {
    cout << results[0] << endl;
    outcomeDisclosure(&results[1], size - 1);
  }
}