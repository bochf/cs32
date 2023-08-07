#include "Wizard.h"

int main() {
  Wizard *wiz[2];
  wiz[0] = new Slytherin();
  wiz[1] = new Wizard();

  for (int khalifa = 0; khalifa < 2; khalifa++) {
    wiz[khalifa]->cast();
    wiz[khalifa]->learn();
  }

  return 0;
}
