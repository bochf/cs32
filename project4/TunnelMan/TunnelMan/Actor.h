#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

#include <memory>
#include <vector>

class StudentWorld;

struct Position {
  int x;
  int y;
};

class Actor : public GraphObject {
 public:
  Actor(StudentWorld* world,
        int imageID,
        const Position& p,
        bool visible = true,
        Direction dir = right,
        unsigned int depth = 2,
        double size = 1.0);
  virtual ~Actor();

  /**
   * @brief the action in each tick
   */
  virtual void doSomething() = 0;

  /**
   * @brief actor is annoyed
   * @param deduction
   */
  virtual void annoyed(int deduction){};

  /**
   * @brief check the actor is alive or not
   * @return
   */
  virtual bool alive() const { return true; }

  /**
   * @brief get the list of positions covered by the actor
   * @param area, an array to store all the positions covered by this actor
   */
  virtual void occupies(std::vector<Position>& area) const;

 protected:
  StudentWorld* world() { return m_world; }

 private:
  Actor() = delete;
  Actor(const Actor&) = delete;

  StudentWorld* m_world;  // pointer to the StudentWorld
};

/**
 * 1. image ID: TID_EARTH.
 * 2. start out facing rightward.
 * 3. Earth has the following graphic parameters:
 *  It has an image depth of 3
 *  It has a size of .25.
 * 4. visible: true
 */
class Earth : public Actor {
 public:
  Earth(StudentWorld* world, const Position& p)
      : Actor(world, TID_EARTH, p, true, GraphObject::right, 3, 0.25){};

  void doSomething() final{};
};
using FIELD = std::vector<std::vector<std::unique_ptr<Earth>>>;

/**
 * @brief class Gold
 * 1. image ID: TID_GOLD
 * 2. start off facing rightward
 * 3. may star out visible or invisible
 * 4. may pickup-able by either TunnelMan or Protesters
 * 5. start out in a permanent state or a temp state
 * 6. Gold Nuggets have the following graphic parameters:
 *  They have an image depth of 2
 *  They have a size of 1.0
 */
class Gold : public Actor {
 public:
  enum class Pickable {
    player,    // pickable by a tunnel man
    protester  // pickable by a protester
  };

  Gold(StudentWorld* world, const Position& p)
      : Actor(world, TID_GOLD, p, false){};

  void doSomething() final;

 private:
  Pickable m_pickable = Pickable::player;
};

class Sonar : public Actor {
 public:
  Sonar(StudentWorld* world, const Position& p) : Actor(world, TID_SONAR, p){};

  void doSomething() final;
};

/**
 * 1. image ID: TID_PLAYER.
 * 2. start at location x=30, y=60.
 * 3. The TunnelMan, in its default state:
 *  a. Is given 10 hit points
 *  b. Is given 5 units of water to squirt with his squirt gun (he may pick up
 * additional Water in the oil field)
 *  c. Is given 1 sonar charge
 *  d. Starts out with zero gold nuggets
 *  e. Should start facing rightward
 * 4. The TunnelMan has the following graphic parameters:
 *  a. It has an image depth of 0
 *  b. It has a size of 1.0
 */
class TunnelMan : public Actor {
 public:
  explicit TunnelMan(StudentWorld* world)
      : Actor(world, TID_PLAYER, {30, 60}), m_golds(){};

  void doSomething() final;
  void annoyed(int points) final;
  bool alive() const final { return m_hitPoints > 0; };

  int waterUnits() const { return m_waterUnits; }
  int sonarCharges() const { return m_sonarCharges; }
  int golds() const { return m_golds.size(); }

 private:
  int m_hitPoints = 10;
  int m_waterUnits = 5;
  int m_sonarCharges = 1;

  std::vector<std::unique_ptr<Gold>> m_golds;

  /**
   * @brief the TunnelMan fires a squirt when a space key is pressed
   */
  void fire();

  /**
   * @brief the TunnelMan moves forward or turn to the direction when an arrow
   * key is pressed
   * @param key
   */
  void move(int key);

  /**
   * @brief abort the current level when ESC key is pressed
   */
  void quit();

  /**
   * @brief the TunnelMan illuminates oil barrels when Z or z is pressed
   */
  void scanOil();

  /**
   * @brief the TunnelMan drop a gold nugget when TAB key is pressed
   */
  void dropGold();

  /**
   * @brief get the new position based on the direction and distance
   * @param dir the direction
   * @param dist the distance from the TunnelMan
   * @return the new corrdinates
   */
  Position newPosition(const Direction& dir, int dist) const;
};

/**
 * 1. Timage ID: TID_BOULDER.
 * 2. A Boulder object must have its x,y location specified for it
 * 3. Boulders start out in a stable state (more on this below).
 * 4. Boulders start out facing downward.
 * 5. Boulders have the following graphic parameters:
 *  a. They have an image depth of 1
 *  b. They have a size of 1.0
 * 6. Visible: true
 */

class Boulder : public Actor {
 public:
  // Boulder state
  enum class State {
    stable,   // If the Boulder is currently in the stable state, then it must
              // check to see if there is any Earth in the 4 squares immediately
              // below it.
    waiting,  // If none of the 4 squares beneath the Boulder have any Earth,
              // then the Boulder must transition into a waiting state and keeps
              // for 30 ticks
    falling,  // If the Boulder is in a waiting state and 30 ticks have elapsed,
              // then it must transition into a falling state
    dead      // The Boulder keeps falling down until it hit the bottom of the
              // field, or the earth, or another boulder
  };

  Boulder(StudentWorld* world, const Position& p)
      : Actor(world, TID_BOULDER, p, true, down, 1){};
  void doSomething() final;
  bool alive() const final { return m_state != State::dead; }

 private:
  State m_state = State::stable;
  int m_waitTicks = 0;

  /**
   * @brief check the Boulder object is above any Earth object
   * @return true if there is any Earth in the 4 squares immediately below it.
   */
  bool onEarth() const;
};

class Squirt : public Actor {
 public:
  Squirt(StudentWorld* world, const Position& p, const Direction dir)
      : Actor(world, TID_WATER_SPURT, p, true, dir, 1){};

  void doSomething() final;

 private:
  int m_distance = 4;
};

class Barrel : public Actor {
 public:
  Barrel(StudentWorld* world, const Position& p)
      : Actor(world, TID_BARREL, p, false){};

  void doSomething() final;
};

class WaterPool : public Actor {
 public:
  WaterPool(StudentWorld* world, const Position& p)
      : Actor(world, TID_WATER_POOL, p){};

  void doSomething() final;
};

class Protester : public Actor {
 public:
  Protester(StudentWorld* world, int tid, const Position& p)
      : Actor(world, tid, p, true, Direction::left){};
};

class RegularProtester : public Protester {
 public:
  RegularProtester(StudentWorld* world, const Position& p)
      : Protester(world, TID_PROTESTER, p){};

  void doSomething() final;
};

class HardcoreProtester : public Protester {
 public:
  HardcoreProtester(StudentWorld* world, const Position& p)
      : Protester(world, TID_HARD_CORE_PROTESTER, p){};

  void doSomething() final;
};

#endif  // ACTOR_H_
