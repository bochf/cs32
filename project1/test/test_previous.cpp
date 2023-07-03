#include "Previous.h"

int main() {
  Previous prev(2, 2);
  prev.dropACrumb(1, 1);
  prev.showPreviousMoves();
}