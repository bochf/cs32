#ifndef __PREVIOUS__
#define __PREVIOUS__

#include "globals.h"

class Previous {
public:
  // Constructor/destructor
  Previous(int nRows, int nCols);
  virtual ~Previous();

  bool dropACrumb(int r, int c);
  void showPreviousMoves() const;

private:
  // DATA
  int m_rows;
  int m_cols;
  int m_grid[MAXROWS][MAXCOLS];

  // Disable default constructors
  Previous() = delete;
  Previous(const Previous &lhs) = delete;
};

#endif