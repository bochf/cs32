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
        int x,
        int y,
        bool visible = true,
        Direction dir = right,
        unsigned int depth = 2,
        double size = 1.0);
  virtual ~Actor();

  /**
   * @brief the action in each tick
   */
  virtual void doSomething(){};

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
  virtual void getArea(std::vector<Position>& area) const;

 protected:
  StudentWorld* m_world;  // pointer to the StudentWorld

 private:
  Actor() = delete;
  Actor(const Actor&) = delete;
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
  Earth(StudentWorld* world, int x, int y)
      : Actor(world, TID_EARTH, x, y, true, GraphObject::right, 3, 0.25){};
};
using FIELD = std::vector<std::vector<std::unique_ptr<Earth>>>;

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
  explicit TunnelMan(StudentWorld* world, FIELD& earthMap)
      : Actor(world, TID_PLAYER, 30, 60), m_earthMap(earthMap){};

  void doSomething() final;
  void annoyed(int points) final;
  bool alive() const final { return m_hitPoints > 0; };

 private:
  int m_hitPoints = 10;
  int m_waterUnits = 5;
  int m_sonarCharges = 1;
  int m_goldNuggets = 0;
  FIELD& m_earthMap;  // reference to the earth map in the game world

  /**
   * @brief dig the earth under the player
   * remove/destroy the Earth objects fromt he 4*4 area occupied by the
   * TunnelMan play a SOUND_DIG
   */
  bool dig() const;
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

  Boulder(StudentWorld* world, int x, int y)
      : Actor(world, TID_BOULDER, x, y, true, down, 1){};
  void doSomething() final;
  bool alive() const final { return m_state != State::dead; }

 private:
  State m_state = State::stable;
  int m_waitTicks = 0;
};

class Squirt : public Actor {
 public:
  Squirt(StudentWorld* world, int x, int y, const Direction dir)
      : Actor(world, TID_WATER_SPURT, x, y, true, dir, 1){};
  void doSomething() final{};

 private:
  int m_distance = 4;
};

class Barrel : public Actor {
 public:
  Barrel(StudentWorld* world, int x, int y)
      : Actor(world, TID_BARREL, x, y, false){};
  void doSomething() final{};
};

class Gold : public Actor {
 public:
  Gold(StudentWorld* world, int x, int y, bool visiable, int pickable)
      : Actor(world, TID_GOLD, x, y, visiable){};
  void doSomething() final{};

 private:
  int m_pickable = 0;
};

class Sonar : public Actor {
 public:
  Sonar(StudentWorld* world, int x, int y) : Actor(world, TID_SONAR, x, y){};
  void doSomething() final{};
};

class WaterPool : public Actor {
 public:
  WaterPool(StudentWorld* world, int x, int y)
      : Actor(world, TID_WATER_POOL, x, y){};
  void doSomething() final{};
};

class Protester : public Actor {
 public:
  Protester(StudentWorld* world, int tid, int x, int y)
      : Actor(world, tid, x, y, true, Direction::left){};
};

class RegularProtester : public Protester {
 public:
  RegularProtester(StudentWorld* world, int x, int y)
      : Protester(world, TID_PROTESTER, x, y){};
  void doSomething() final{};
};

class HardcoreProtester : public Protester {
 public:
  HardcoreProtester(StudentWorld* world, int x, int y)
      : Protester(world, TID_HARD_CORE_PROTESTER, x, y){};
  void doSomething() final{};
};

#endif  // ACTOR_H_
