#ifndef __UTILS__
#define __UTILS__

#include "anagrams.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

/// @brief the condition used for filtering the cache. Each string in the cache
/// is used as the input parameter, return the result of the condition defined
/// in the body
using COND = std::function<bool(const std::string &word)>;
class Collector {
public:
  Collector(const std::string dict[], int size, const std::string &word) {
    copy(dict, size, word.length());
  };

  /// @brief check the result is full
  /// @return true if size of the result >= MAXRESULTS
  bool full() const { return m_results.size() >= MAXRESULTS; };

  /// @brief check and add a word into result if it exists in the dictionary and
  /// not in the result
  /// @param word
  void add(const std::string &word);

  /// @brief find a word starts with a prefix in an array
  /// @param word the prefix to match
  /// @param list the list of words
  /// @param size the number of words in the array
  /// @param steps number of words to process in a batch
  /// @return true if found
  bool partialMatch(const std::string &word);

  /// @brief copy the result
  /// @param results the outside array to store the result
  /// @return number of elements copied
  size_t dumpResult(std::string results[]);

private:
  Collector(const Collector &)  = delete;
  Collector(const Collector &&) = delete;

  std::vector<std::string> m_results;
  std::string              m_cache[MAXDICTWORDS];
  size_t                   m_cacheSize = 0;

  void copy(const std::string dict[], int size, size_t len);

  /// @brief find a word in an array
  /// @param cond the condition to match
  /// @param list the list of words
  /// @param size the number of words in the array
  /// @param steps number of words to process in a batch
  /// @return true if found
  bool search(const COND       &cond,
              const std::string list[],
              size_t            size,
              size_t            steps = 1);

  /// @brief find a word in an array
  /// @param word the word to search
  /// @param list the list of words
  /// @param size the number of words in the array
  /// @param steps number of words to process in a batch
  /// @return true if found
  bool match(const std::string &word,
             const std::string  list[],
             size_t             size,
             size_t             steps = 1);
};

/// @brief read words from a file
/// @param dictfile the input file stream
/// @param dict the output list
/// @param maxSize max size of the output array
/// @return number of words stored in the array
int fillDict(std::istream &dictfile, std::string dict[], size_t maxSize);

/// @brief generate all permutations of a word
/// @param resultProcessor a callable function to deal with the permutation
/// @param word the original input
/// @param pos the position of the character to be switched with
void permutate(Collector &resultProcessor, std::string &word, int pos);

using ITERATOR = std::function<void(size_t &)>;
/// @brief recursively execute an operation
/// @param iterator the executor
/// @param from the start point
/// @param to the end point, exclusive
void repeat(const ITERATOR &iterator, size_t from, size_t to);

#endif