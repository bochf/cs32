#ifndef __FIVEFORFIGHTING__
#define __FIVEFORFIGHTING__

#include <iostream>
#include <string>

template <class T> void fiveForFighting(const T &num);

void fiveForFighting(const int &num) {
  std::cout << "If I 'm given " << num
            << ", then it' s five times the fun to get " << num * 5
            << std::endl;
}

void fiveForFighting(const double &num) {
  std::cout << "If I 'm given " << num
            << ", then it' s five times the fun to get " << num * 5
            << std::endl;
}

void fiveForFighting(const std::string &num) {
  std::cout << "If I 'm given " << num
            << ", then it' s five times the fun to get ";

  for (int i = 0; i < 5; ++i) {
    std::cout << num;
  }
  std::cout << std::endl;
}

#endif