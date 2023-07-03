#ifndef __GAME__
#define __GAME__

class Arena;

class Game {
public:
  // Constructor/destructor
  Game(int rows, int cols, int nRobots);
  ~Game();

  // Mutators
  void play();

private:
  Arena *m_arena;
};

#endif