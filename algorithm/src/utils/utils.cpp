#include "utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace CS32 {
const uint64_t KILO = 1 << 10;
const uint64_t MEGA = 1 << 20;
const uint64_t GIGA = 1 << 30;

string humanize(uint64_t num) {
  ostringstream oss;
  if (num > GIGA)
    oss << fixed << setprecision(3) << 1.0 * num / GIGA << " G";
  else if (num > MEGA)
    oss << fixed << setprecision(3) << 1.0 * num / MEGA << " M";
  else if (num > KILO)
    oss << fixed << setprecision(3) << 1.0 * num / KILO << " K";
  else
    oss << num;
  return oss.str();
}

void printData(const vector<int> &data) {
  for (int value : data) {
    cout << value << " ";
  }
  cout << endl;
}
} // namespace CS32
