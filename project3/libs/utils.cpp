#include "utils.h"
#include "anagrams.h"

#include <iostream>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////
// helper functions

/// @brief read words from a file
/// @param dictfile the input file stream
/// @param dict the output list
/// @param maxSize max size of the output array
/// @return number of words stored in the array
int fillDict(istream &dictfile, string dict[], size_t maxSize) {
  if (dictfile.eof() || maxSize == 0) {
    return 0;
  }

  if (getline(dictfile, dict[0])) {
    return fillDict(dictfile, &dict[1], maxSize - 1) + 1;
  }

  return 0;
}

void repeat(const function<void(int)> &iterator, int from, int to) {
  if (from >= to)
    return;

  iterator(from);
  repeat(iterator, from + 1, to);
}

/// @brief find a word in an array
/// @param word the word to search
/// @param dict the list of words
/// @param size the number of words in the array
/// @return position of the word in the array, -1 if not found
bool searchWord(const string &word, const string dict[], int size, int step) {

  if (0 == size) {
    return false;
  }

  if (size > step && step > 100) {
    if (searchWord(word, dict, step)) {
      return true;
    }
    return searchWord(word, &dict[step], size - step, step);
  }

  if (word == dict[size - 1]) {
    return true;
  }

  return searchWord(word, dict, size - 1, step);
}

void permutate(const function<int(const string &)> &resultProcessor,
               string                              &word,
               int                                  pos) {
  if (pos + 1 >= word.length()) {
    resultProcessor(word);
    return;
  }

  // Reorg reorg(word, pos);
  auto iterator = [&resultProcessor, &word, &pos](int index) {
    swap(word[pos], word[index]);
    permutate(resultProcessor, word, pos + 1);
    swap(word[pos], word[index]);
  };
  permutate(resultProcessor, word, pos + 1);
  repeat(iterator, pos + 1, word.length());
}
