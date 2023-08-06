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

void repeat(const ITERATOR &iterator, size_t from, size_t to) {
  if (from >= to)
    return;

  iterator(from);
  repeat(iterator, from + 1, to);
}

void permutate(Collector &collector, string &word, int pos) {
  if (collector.full()) {
    return;
  }

  if (pos + 1 >= word.length()) {
    collector.add(word);
    return;
  }
  if (!collector.partialMatch(word.substr(0, pos + 1))) {
    // return earlier if the prefix can't be found in the dictionary
    // no need to try the combinations under the same prefix
    return;
  }

  auto iterator = [&collector, &word, &pos](int index) {
    swap(word[pos], word[index]);
    permutate(collector, word, pos + 1);
    swap(word[pos], word[index]);
  };
  // permutate(collector, word, pos + 1);
  repeat(iterator, pos, word.length());
}

void Collector::add(const string &word) {
  if (!match(word, m_results.data(), m_results.size()) // not in the result
      && match(word, m_cache, m_cacheSize, 100)        // in the dictionary
  ) {
    m_results.push_back(word);
  }
}

size_t Collector::dumpResult(string results[]) {
  auto dumper = [this, results](int i) { results[i] = m_results[i]; };
  repeat(dumper, 0, m_results.size());
  return m_results.size();
}

void Collector::copy(const string dict[], int size, size_t len) {
  if (size > 0) {
    if (const string &word = dict[size - 1]; word.length() == len) {
      m_cache[m_cacheSize++] = word;
    }
    copy(dict, size - 1, len);
  }
}

bool Collector::match(const string  &word,
                      const ::string list[],
                      size_t         size,
                      size_t         steps) {
  auto wholeWordMatch = [&word](const string &value) { return word == value; };
  return search(wholeWordMatch, list, size, steps);
}

bool Collector::partialMatch(const string &word) {
  auto prefixMatch = [&word](const string &value) {
    return word == value.substr(0, word.length());
  };
  return search(prefixMatch, m_cache, m_cacheSize, 100);
}

bool Collector::search(const COND  &cond,
                       const string list[],
                       size_t       size,
                       size_t       steps) {
  if (0 == size) {
    return false;
  }

  if (size > steps && steps > 64) {
    if (search(cond, list, steps)) {
      return true;
    }
    return search(cond, &list[steps], size - steps, steps);
  }

  if (cond(list[size - 1])) {
    return true;
  }

  return search(cond, list, size - 1, steps);
}
