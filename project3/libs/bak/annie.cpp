#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

// A new function that helps to write into "dict", also making sure the number
// of lines do not exceed MAXDICTWORDS.
void countLines(istream &dictfile, string dict[], int &linecount) {
  if (linecount >= MAXDICTWORDS) {
    cout << "Word count exceeds max!" << endl;
    exit(1);
  }
  if (!dictfile)
    return; // Stop when dictfile is read up.
  dictfile >> dict[0];
  linecount++;
  countLines(dictfile, dict + 1, linecount);
}

// Puts each string in dictfile into the array dict. Returns the number of words
// read into dict.This number should not be larger than MAXDICTWORDS since that
// is the size of the array.
int vocabularyCreator(istream &dictfile, string dict[]) {
  int linecount = 0;
  countLines(dictfile,
             dict,
             linecount); // Use the countLines function to write "dict".
  return linecount - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     For each character in rest{
//       Add the character to the end of prefix.
//       Remove character from rest.
//       Use recursion to generate permutations with the updated values for
//       prefix and rest.
//     }
void loop(size_t i, size_t max, string prefix, string rest) {
  if (i >= max) {
    return;
  }
  std::string newPrefix = prefix + rest.substr(i, 1);
  std::string newRest   = rest.substr(0, i) + rest.substr(i + 1);
  i++;
  loop(i, newRest.length(), newPrefix, newRest);
}

// print permutations of string
//
// void printPermutations(string prefix, string rest) {
// if (rest is empty) {
//    Display the prefix string.
// }
// else {
//     For each character in rest{
//       Add the character to the end of prefix.
//       Remove character from rest.
//       Use recursion to generate permutations with the updated values for
//       prefix and rest.
//     }
// }
// }
void printPermutations(vector<string> &result,
                       const string   &word,
                       string          prefix,
                       string          rest) {
  if (rest.empty()) {
    result.push_back(prefix);
    return;
  } else {
    loop(0, rest.length(), prefix, rest);
  }
}

// given "word", match it to the "dict" from the last word of "dict". If there
// is a match, increase the counter and write into results.
void match(string word, const string dict[], int size, string results[]) {
  if (size == 0) {
    return; // when the size of dictionary becomes 0, it means no matches. So
            // just return.
  }
  if (word == dict[size - 1]) // When there is a match
  {
    if (results->size() == MAXRESULTS) {
      return;
    }
    results[results->size()] = word; // write into results
    return;
  } else {
    match(word, dict, size - 1, results);
  }
}

void searchDict(size_t         i,
                size_t         max,
                vector<string> permutations,
                const string   dict[],
                int            size,
                string         results[]) {
  if (i >= max) {
    return;
  } else {
    match(permutations[i], dict, size, results);
  }
  searchDict(i + 1, max, permutations, dict, size, results);
}

// Puts all the possibilities of word which are found in dict into results.
// Returns the number of matched words found.This number should not be larger
// than MAXRESULTS since that is the size of the array.The size is the number of
// words inside the dict array.
int potentialSequences(string       word,
                       const string dict[],
                       int          size,
                       string       results[]) {
  vector<string> permutations;

  printPermutations(permutations, word, "", word);

  searchDict(0, permutations.size(), permutations, dict, size, results);

  return results->size();
}

// Displays size number of strings from results. The results can be printed in
// any order.
void outcomeDisclosure(const string results[], int size) {
  if (size < 0) {
    return;
  }
  cout << results[--size] << endl;
  outcomeDisclosure(results, size);
}

/*int main()
{
        string results[MAXRESULTS];
        string dict[MAXDICTWORDS];
        ifstream dictfile;         // file containing the list of words
        int nwords;                // number of words read from dictionary
        string word;

        dictfile.open("words.txt");
        if (!dictfile) {
                cout << "File not found!" << endl;
                return (1);
        }

        nwords = vocabularyCreator(dictfile, dict);

        cout << "Please enter a string for an anagram: ";
        cin >> word;

        int numMatches = potentialSequences(word, dict, nwords, results);
        if (!numMatches) {
                cout << "No matches found" << endl;
        }
        else {
                outcomeDisclosure(results, numMatches);
        }

        return 0;
}
*/