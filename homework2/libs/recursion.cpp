#include "recursion.h"

#include <cassert>
#include <sstream>
#include <string>

using namespace std;
/*
int productOfTheSystem(unsigned int m, unsigned int n) {
  if (0 == m || 0 == n)
    return 0;

  if (n == 1)
    return m;

  return productOfTheSystem(m, n - 1) + m;
}

int accountForADigit(int num, int digit) {
  assert(0 <= digit && digit < 10);

  if (num != 0) {
    int last = num % 10;
    return accountForADigit(num / 10, digit) +
           ((digit * digit == last * last) ? 1 : 0);
  }

  return 0;
}

string eightClap(string n) {
  if (n.length() < 2) {
    return n;
  }

  string prefix(n.substr(0, 1));
  if (n[0] == n[1]) {
    prefix += "88";
  }
  return prefix + eightClap(n.substr(1));
}

string coneHeads(string str) {
  if (str.empty())
    return "";

  size_t last = str.length() - 1;
  if (str[0] != '<')
    return coneHeads(str.substr(1));
  if (str[last] != '>')
    return coneHeads(str.substr(0, last));

  return str;
}

// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
// Pseudocode Example:
//  conglomerateOfNumbers({2, 4, 8}, 3, 10) => true
//  conglomerateOfNumbers({2, 4, 8}, 3, 6)  => true
//  conglomerateOfNumbers({2, 4, 8}, 3, 11) => false
//  conglomerateOfNumbers({2, 4, 8}, 3, 0)  => true
//  conglomerateOfNumbers({}, 0, 0)         => true
// bool conglomerateOfNumbers(const int a[], int size, int target) {
//   if (0 == target) {
//     return true;
//   }

//   if (0 == size) {
//     return target == 0;
//   }

//   if (a[size - 1] == target) {
//     return true;
//   }

//   if (conglomerateOfNumbers(a, size - 1, target)) {
//     return true;
//   }

//   return conglomerateOfNumbers(a, size - 1, target - a[size - 1]);
// }
*/

bool findAWay(string maze[],
              int    nRows,
              int    nCols,
              int    sr,
              int    sc,
              int    er,
              int    ec) {
  { // validate the maze
    ostringstream oss;
    oss << "each row must have " << nCols << " chars.\0";
    const string validRow = oss.str();

    assert(("the maze must have more than 2 rows.", nRows > 2));
    assert(("the maze must have more than 2 columns.", nCols > 2));
    for (size_t r = 0; r < nRows; ++r) {
      const auto &row = maze[r];
      assert((validRow, row.size() == nCols));
      assert(("the first char of each row must be 'X'", row[0] == 'X'));
      assert(("the last char of each row must be 'X'", row[nCols - 1] == 'X'));
    }
    assert(("the first row must be all 'X'", maze[0] == string(nCols, 'X')));
    assert(("the last row must be all 'X'",
            maze[nRows - 1] == string(nCols, 'X')));
  }

  { // validate the start and end points
    assert(("start point must be in the maze", maze[sr][sc] == '.'));
    assert(("end point must be in the maze", maze[er][ec] == '.'));
  }

  if (sr == er && sc == ec) {
    maze[er][ec] = 'E';
    return true; // reaches the destination
  }

  maze[sr][sc] = 'V'; // mark the start point visited

  // go right
  if (maze[sr][sc + 1] == '.' &&
      findAWay(maze, nRows, nCols, sr, sc + 1, er, ec)) {
    return true;
  }

  // go down
  if (maze[sr + 1][sc] == '.' &&
      findAWay(maze, nRows, nCols, sr + 1, sc, er, ec)) {
    return true;
  }

  // go left
  if (maze[sr][sc - 1] == '.' &&
      findAWay(maze, nRows, nCols, sr, sc - 1, er, ec)) {
    return true;
  }

  // go up
  if (maze[sr - 1][sc] == '.' &&
      findAWay(maze, nRows, nCols, sr - 1, sc, er, ec)) {
    return true;
  }

  maze[sr][sc] = 'U';
  return false;
}
