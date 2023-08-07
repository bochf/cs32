#ifndef __WIZARD__
#define __WIZARD__

#include <iostream>

class Wizard {
public:
  Wizard() {
    std::cout << "Wizard object " << this << " is created." << std::endl;
  }

  virtual ~Wizard() {
    std::cout << "Wizard object " << this << " is destroyed." << std::endl;
  }

  void learn() { /* Implementation not shown */
  }
  // make cast() a virtual function

  virtual void cast(){};
};

class Slytherin : public Wizard {
public:
  Slytherin() {
    std::cout << "Slytherin object " << this << " is created." << std::endl;
  }

  ~Slytherin() final {
    std::cout << "Wizard object " << this << " is destroyed." << std::endl;
  }

  void cast() override{};
};

#endif