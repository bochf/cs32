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

 private:
  std::unique_ptr<TunnelMan> m_player;
  std::vector<std::unique_ptr<Actor>> m_actors;
  FIELD m_earthMap;
  int m_numBoulders = 0;
  int m_numGolds = 0;
  int m_numBarrels = 0;
  int m_numProtesters = 0;

  void createEarth();
  void createBoulders();
  void createGolds();

  /**
   * @brief update the statistic info of the game
   */
  void updateDisplayText();

  bool levelCompleted() { return false; }
  void removeDeadGameObjects(){};
};

#endif  // STUDENTWORLD_H_
