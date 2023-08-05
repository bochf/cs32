#ifndef __UTILS__
#define __UTILS__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

/// @brief read words from a file
/// @param dictfile the input file stream
/// @param dict the output list
/// @param maxSize max size of the output array
/// @return number of words stored in the array
int fillDict(std::istream &dictfile, std::string dict[], size_t maxSize);

/// @brief find a word in an array
/// @param word the word to search
/// @param dict the list of words
/// @param size the number of words in the array
/// @param step number of words to process in a batch
/// @return true if found
bool searchWord(const std::string &word,
                const std::string  dict[],
                int                size,
                int                step = 1);

/// @brief generate all permutations of a word
/// @param resultProcessor a callable function to deal with the permutation
/// @param word the original input
/// @param pos the position of the character to be switched with
void permutate(const std::function<int(const std::string &)> &resultProcessor,
               std::string                                   &word,
               int                                            pos);

/// @brief recursively execute an operation
/// @param iterator the executor
/// @param from the start point
/// @param to the end point, exclusive
void repeat(const std::function<void(int)> &iterator, int from, int to);

#endif