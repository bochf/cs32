#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

using ACTORS = std::vector<std::unique_ptr<Actor>>;

class StudentWorld : public GameWorld {
 public:
  explicit StudentWorld(const std::string& assetDir) : GameWorld(assetDir) {}

  virtual int init();

  virtual int move();

  virtual void cleanUp();

  ///
  /// new methods not inherited from GameWorld
  ///
  /**
   * @brief check the coordinates of the bottom left corner of a 4 * 4  object
   * is valid or not, it must be in the oil field
   * @param pos the coordinates
   * @return true if in the oil field
   */
  bool validPosition(const Position& pos) const {
    return (pos.x >= 0 && pos.x <= VIEW_WIDTH - 4 && pos.y >= 0 &&
            pos.y <= VIEW_HEIGHT - 4);
  }

  /**
   * @brief check an actor can move to the destination
   * the location must be inside the field and greater than 3 away from the
   * center of a Boulder
   * @param x
   * @param y
   * @return true if the TunnelMan can go there
   */
  bool noBoulder(const Position& pos) const;

  /**
   * @brief search all the hidden objects (e.g. oil barrels and gold nuggets)
   * within the radius of a given position and make them visible.
   * @param radius the radius
   * @return number of barrels found
   */
  int discover(int tid, int radius);

  /**
   * @brief remove any Earth object in the specified area
   * @param bottomLeft, the coordinates of the bottom left corner of the square,
   * inclusive
   * @param topRight, the coordinates of the top right corner of the square,
   * exclusive
   */
  bool removeEarth(const Position& bottomLeft, const Position& topRight);

  /**
   * @brief check any Earth object in the specified area
   * @param bottomLeft, the coordinates of the bottom left corner of the square,
   * inclusive
   * @param topRight, the coordinates of the top right corner of the square,
   * exclusive
   * @return true if there are Earth objects
   */
  bool hasEarth(const Position& bottomLeft, const Position& topRight) const;

  ACTORS& getActors(int tid) { return m_actors.at(tid); };
  const ACTORS& getActors(int tid) const { return m_actors.at(tid); };
  TunnelMan& getPlayer() { return *m_player; }
  const TunnelMan& getPlayer() const { return *m_player; }

  void onBoulderFall(const Boulder& boulder);

 private:
  std::unique_ptr<TunnelMan> m_player;
  std::unordered_map<int, ACTORS> m_actors;

  std::array<std::array<std::unique_ptr<Earth>, VIEW_WIDTH>, VIEW_HEIGHT>
      m_earthMap;

  struct ProtesterProperty {
    // set values by the current game level
    void reset(int level) {
      m_interval = std::max(25, 200 - level);
      m_max = std::min(15.0, 2 + level * 1.5);
      m_hardcoreProbability = std::min(90, level * 10 + 30);
      m_wait = 0;
    }

    void resetWaitTime() { m_wait = m_interval; }

    // A new Protester (Regular or Hardcore) may only be added to the oil field
    // after at least T ticks have passed since the last Protester of any type
    // was added, where : int T = max(25, 200 ¨C current_level_number)
    int m_interval = 200;

    // The target number P of Protesters that should be on the oil field is
    // equal to: int P = min(15, 2 + current_level_number * 1.5)
    double m_max = 2;

    // The odds of  the Protester being a Hard Core  Protester(vs.a Regular
    // Protester) must be determined with this formula
    // probabilityOfHardcore = min(90, current_level_number * 10 + 30)
    int m_hardcoreProbability = 30;

    // wait for number of ticks to create a new protester if not full
    int m_wait = 0;
  };
  ProtesterProperty m_pp;

  //  There is a 1 in G chance that a new Water Pool or Sonar Kit Goodie will be
  //  added to the oil field during any particular tick,
  int m_goodieChance = 300;

  /**
   * @brief randomly find a valid position to put a Boulder, Gold Nugget and
   * Oil Barrel
   * @param left, right, bottom, top define the rectangle the valid
   * position, inclusive
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
  int protesters() const { return m_actors.at(TID_PROTESTER).size(); }

  /**
   * @brief update the statistic info of the game
   */
  void updateDisplayText();

  /**
   * @brief check the current level completed;
   */
  bool levelCompleted() const;

  /**
   * @brief add new Protesters (Regular or Hardcore) and/or Goodies (Water Pools
   * or Sonar Kits) to the oil field.
   */
  void addNewObjects();

  void addProtester();
  void addWaterPool();
  void addSonar();
  /**
   * @brief remove all the dead objects
   */
  void removeDeadGameObjects();
};

#endif  // STUDENTWORLD_H_
