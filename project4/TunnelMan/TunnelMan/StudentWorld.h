#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"

#include <memory>
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
 public:
  explicit StudentWorld(const std::string& assetDir) : GameWorld(assetDir) {}

  virtual int init();

  virtual int move();

  virtual void cleanUp();

  ///
  /// new methods not inherited from GameWorld
  ///
  FIELD& getField() { return m_earthMap; }

  void addActor(std::unique_ptr<Actor> actor) {
    m_actors.push_back(std::move(actor));
  }

  /**
   * @brief check the TunnelMan can move to the position or not
   * the position must be inside the field and greater than 3 away from the
   * center of a Boulder
   * @param x
   * @param y
   * @return true if the TunnelMan can go there
   */
  bool walkable(int x, int y) const;

  /**
   * @brief remove a squre of earth objects from the field
   * @param x, left
   * @param y, bottom
   * @param size, length of the squire
   * @return number of earth objects been removed
   */
  int removeEarth(int x, int y, int size);

 private:
  std::unique_ptr<TunnelMan> m_player;
  std::vector<std::unique_ptr<Actor>> m_actors;
  FIELD m_earthMap;
  int m_numBoulders = 0;
  int m_numGolds = 0;
  int m_numBarrels = 0;
  int m_numProtesters = 0;

  /**
   * @brief randomly find a valid position to put a Boulder, Gold Nugget and Oil
   * Barrel
   * @param left, right, bottom, top define the rectangle the valid position,
   * inclusive
   * @param radius the minimal distance of 2 objects
   * @return position
   */
  Position generatePosition(int left,
                            int right,
                            int bottom,
                            int top,
                            int radius) const;

  void createEarth();
  void createBoulders();
  void createGolds();
  void createBarrels();

  /**
   * @brief update the statistic info of the game
   */
  void updateDisplayText();

  bool levelCompleted() { return false; }
  void removeDeadGameObjects(){};
};

#endif  // STUDENTWORLD_H_
