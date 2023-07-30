#include "recursion.h"
#include <string>

using namespace std;

// my lap isn't working T_T

// Returns the product of two non-negative integers, m and n, using only
// repeated addition.
int productOfTheSystem(unsigned int m, unsigned int n) {
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return m;
  } else {
    return m + productOfTheSystem(m, n - 1);
  }
}

// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
//    accountForADigit(18838, 8) => 3
//    accountForADigit(55555, 3) => 0
//    accountForADigit(0, 0)     => 0 or 1 (either if fine)
//
int accountForADigit(int num, int digit) {
  if (num == 0) {
    return 0;
  }
  int x = num % 10;
  if (x * x == digit * digit) {
    return accountForADigit(num / 10, digit) + 1;
  }
  return accountForADigit(num / 10, digit);
}

// Returns a string where the same characters next each other in
// string n are separated by "88" (you can assume the
// input  string will not have 8’s)
//
// Pseudocode Example:
//    eightClap("goodbye") => "go88odbye"
//    eightClap("yyuu")    => "y88yu88u"
//    eightClap("aaaa")    => "a88a88a88a"
//
string eightClap(string n) {
  if (n.size() < 2) {
    return n;
  } else if (n.substr(0, 1) == n.substr(1, 1)) {
    return n.substr(0, 1) + "88" + eightClap(n.substr(1));
  } else {
    return n.substr(0, 1) + eightClap(n.substr(1));
  }
}

// str contains a single pair of the less than and greater than
// signs, return a new string made of only the less than and
// greater than sign and whatever is contained in between
//
//  Pseudocode Example:
//     coneHeads("abc<ghj>789") => "<ghj>"
//     coneHeads("<x>7")        => "<x>"
//     coneHeads("4agh<y>")    => "<y>"
//     coneHeads("4agh<>")     => "<>"
//
string coneHeads(string str) {
  long s = str.size() - 1;
  if (str.size() < 2) {
    return "";
  }

  if (str[0] != '<') {
    return coneHeads(str.substr(1));
  }
  if (str[s] != '>') {
    return coneHeads(str.substr(0, s));
  }
  return str;
}

// Return true if the total of any combination of elements in the
// array a equals the value of the target.
//
//  Pseudocode Example:
//     conglomerateOfNumbers({2, 4, 8}, 3, 10) => true
//     conglomerateOfNumbers({2, 4, 8}, 3, 6)  => true
//     conglomerateOfNumbers({2, 4, 8}, 3, 11) => false
//     conglomerateOfNumbers({2, 4, 8}, 3, 0)  => true
//     conglomerateOfNumbers({}, 0, 0)         => true
//
bool conglomerateOfNumbers(const int a[], int size, int target) {
  if (target == 0) {
    return true;
  }
  if (size == 0) {
    return target == 0;
  }
  int s = size - 1;
  if (a[s] == target) {
    return true;
  }
  if (!conglomerateOfNumbers(a, s, target)) {
    return conglomerateOfNumbers(a, s, target - a[s]);
  } else {
    return true;
  }
}

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool findAWay(string maze[],
              int    nRows,
              int    nCols,
              int    sr,
              int    sc,
              int    er,
              int    ec) {
  if (sr == er && sc == ec) {
    return true;
  }

  if (maze[sr][sc] != '.') {
    return false;
  }

  maze[sr][sc] = 'v';
  if (findAWay(maze, nRows, nCols, sr - 1, sc, er, ec)) { // go up
    return true;
  }

  if (findAWay(maze, nRows, nCols, sr, sc - 1, er, ec)) { // go left
    return true;
  }

  if (findAWay(maze, nRows, nCols, sr + 1, sc, er, ec)) { // go down
    return true;
  }

  if (findAWay(maze, nRows, nCols, sr, sc + 1, er, ec)) { // go right
    return true;
  }

  maze[sr][sc] = 'u';
  return false;
}