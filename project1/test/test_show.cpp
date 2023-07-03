#include "Arena.h"
#include "Player.h"
#include "Previous.h"
#include "globals.h"

int main() {
  Arena a(2, 2);
  a.addPlayer(1, 1);
  a.player()->move(RIGHT);
  a.player()->stand();
  a.player()->move(DOWN);
  a.thePrevious().showPreviousMoves();
}