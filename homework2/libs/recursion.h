#pragma once

#include <string>

// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int productOfTheSystem(unsigned int m, unsigned int n);

// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
// accountForADigit(18838, 8) => 3
// accountForADigit(55555, 3) => 0
// accountForADigit(0, 0)     => 0 or 1 (either if fine)
//
int accountForADigit(int num, int digit);

// Returns a string where the same characters next each other in
// string n are separated by "88" (you can assume the input
// string will not have 8’s)
//
// Pseudocode Example:
//  eightClap("goodbye") => "go88odbye"
//  eightClap("yyuu") => "y88yu88u"
//  eightClap("aaaa") => "a88a88a88a"
//
std::string eightClap(std::string n);

// str contains a single pair of the less than and greater than
// signs, return a new string made of only the less than and
// greater than sign and whatever is contained in between
//
// Pseudocode Example:
//  coneHeads("abc<ghj>789") => "<ghj>"
//  coneHeads("<x>7") => "<x>"
//  coneHeads("4agh<y>") => "<y>"
//  coneHeads("4agh<>") => "<>"
std::string coneHeads(std::string str);

// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
// Pseudocode Example:
//  conglomerateOfNumbers({2, 4, 8}, 3, 10) => true
//  conglomerateOfNumbers({2, 4, 8}, 3, 6)  => true
//  conglomerateOfNumbers({2, 4, 8}, 3, 11) => false
//  conglomerateOfNumbers({2, 4, 8}, 3, 0)  => true
//  conglomerateOfNumbers({}, 0, 0)         => true
bool conglomerateOfNumbers(const int a[], int size, int target);

/// @brief find a way in the maze from start to end point
/// @param maze
/// @param nRows number of rows, height of the maze
/// @param nCols number of columns, width of the maze
/// @param sr row of start
/// @param sc column of start
/// @param er row of end
/// @param ec row of column
/// @return true if start and end are connectible
bool findAWay(std::string maze[],
              int         nRows,
              int         nCols,
              int         sr,
              int         sc,
              int         er,
              int         ec);
