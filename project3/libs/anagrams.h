#ifndef __ANAGRAMS__
#define __ANAGRAMS__

#include <fstream>
#include <string>

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

/// @brief Puts each string in dictfile into the array dict.
/// since that is the size of the array.
/// @param dictfile the file stream
/// @param dict words list
/// @return Returns the number of words read into dict. This number should not
/// be larger than MAXDICTWORDS
int vocabularyCreator(std::istream &dictfile, std::string dict[]);

/// @brief Puts all the possibilities of word which are found in dict into
/// results.
/// @param word the word to be matched
/// @param dict the list of all the words
/// @param size number of words inside the dict array
/// @param results the list of matched words
/// @return the number of matched words found. This number should not be larger
/// than MAXRESULTS since that is the size of the array.
int potentialSequences(std::string       word,
                       const std::string dict[],
                       int               size,
                       std::string       results[]);

/// @brief Displays size number of strings from results. The results can be
/// printed in any order.
/// @param results the string array
/// @param size number of strings to be printed
void outcomeDisclosure(const std::string results[], int size);

#endif
