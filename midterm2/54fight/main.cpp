#include "fiveForFighting.h"

#include <iostream>
#include <string>
using namespace std;

int main() {
  string test   = "yum";
  int    num    = 42;
  double numTwo = 3.14;

  fiveForFighting(num);
  fiveForFighting(numTwo);
  fiveForFighting(test);

  return 0;
}
