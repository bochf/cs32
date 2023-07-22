#include "recursion.h"

#include <cassert>
#include <string>

using namespace std;

// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int productOfTheSystem(unsigned int m, unsigned int n) {
  if (0 == m || 0 == n)
    return 0;

  if (n == 1)
    return m;

  return productOfTheSystem(m, n - 1) + m;
}

// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
// accountForADigit(18838, 8) => 3
// accountForADigit(55555, 3) => 0
// accountForADigit(0, 0)     => 0 or 1 (either if fine)
//
int accountForADigit(int num, int digit) {
  assert(0 <= digit && digit < 10);

  if (num != 0) {
    int last = num % 10;
    return accountForADigit(num / 10, digit) +
           ((digit * digit == last * last) ? 1 : 0);
  }

  return 0;
}

// Returns a string where the same characters next each other in
// string n are separated by "88" (you can assume the input
// string will not have 8’s)
//
// Pseudocode Example:
//  eightClap("goodbye") => "go88odbye"
//  eightClap("yyuu") => "y88yu88u"
//  eightClap("aaaa") => "a88a88a88a"
//
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

// str contains a single pair of the less than and greater than
// signs, return a new string made of only the less than and
// greater than sign and whatever is contained in between
//
// Pseudocode Example:
//  coneHeads("abc<ghj>789") => "<ghj>"
//  coneHeads("<x>7") => "<x>"
//  coneHeads("4agh<y>") => "<y>"
//  coneHeads("4agh<>") => "<>"
string coneHeads(string str) {
  return "*"; // This is incorrect.
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
bool conglomerateOfNumbers(const int a[], int size, int target) {
  return false; // This is not always correct.
}

bool findAWay(string maze[],
              int    nRows,
              int    nCols,
              int    sr,
              int    sc,
              int    er,
              int    ec) {
  return false;
}
