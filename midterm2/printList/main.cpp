#include <iostream>
#include <list>
#include <vector>
using namespace std;

void printList(const list<double>&);

int main() {
  vector<int>  vec;
  list<double> li;

  for (int i = 0; i < 5; i++) {
    vec.push_back(i);
    li.push_back(i + 0.5);
  }

  // Erase each element one by one, beginning with the
  // last
  while (!vec.empty()) {
    cout << vec.back() << endl;
    vec.pop_back();
  }
  cout << "vec size: " << vec.size() << endl;
  cout << "vec empty: " << vec.empty() << endl;

  printList(li);

  return 0;
}

void printList(const list<double> &l) {
  // Print out the list
  for (const auto d : l) {
    cout << d << endl;
  }
}
