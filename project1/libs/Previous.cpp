#include "Previous.h"
#include "globals.h"

#include <iostream>

using namespace std;

/// @brief letter to indicate how many times a player has been on the grid point
/// @param age times a player has been on the grid point
/// @return '.' if the player has never be here
///         'A' through 'Y' means 1 through 25 times (A means 1, B means 2,
///         etc.) 'Z' means 26 or more times
char ageSign(int age) {
  if (age <= 0)
    return '.';
  if (age >= 26)
    return 'Z';

  return 'A' + age - 1;
}

Previous::Previous(int nRows, int nCols)
    : m_rows(nRows), m_cols(nCols), m_grid{} {}

Previous::~Previous() {}

bool Previous::dropACrumb(int r, int c) {
  // validate input, 1 <= r <= MAXROWS, 1 <= c <= MAXCOLS
  if (r < 1 || r > MAXROWS || c < 1 || c > MAXCOLS)
    return false;

  ++m_grid[r - 1][c - 1];
  return true;
}

void Previous::showPreviousMoves() const {
  clearScreen();

  for (int r = 0; r < m_rows; ++r) {
    for (int c = 0; c < m_cols; ++c) {
      cout << ageSign(m_grid[r][c]);
    }
    cout << endl;
  }
  cout << endl;
}