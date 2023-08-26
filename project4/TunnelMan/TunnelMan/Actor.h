#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

#include <memory>
#include <queue>
#include <vector>

#define SQUARE(x) (x) * (x)

class StudentWorld;

struct Position {
  int x;
  int y;

  Position(int xx, int yy) : x(xx), y(yy) {}

  // return true if both x and y less than the other
  bool operator<(const Position& other) const {
    return (x < other.x && y < other.y);
  }

  bool operator==(const Position& other) const {
    return (x == other.x && y == other.y);
  }

  // a position that vertically and horizontally size away
  Position operator+(double size) const {
    return Position{static_cast<int>(x + size), static_cast<int>(y + size)};
  }
};

class Actor : public GraphObject {
 public:
  Actor(StudentWorld* world,
        int tid,
        const Position& pos,
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
  virtual void annoyed(int points){};

  /**
   * @brief check the actor is alive or not
   * @return
   */
  virtual bool alive() const { return true; }

  /**
   * @brief an actor pickup a gold
   */
  virtual void pickupGold() {
    // to be overrided in sub classes, the default method is no-op
  }

  Position bottomLeft() const { return Position(getX(), getY()); }
  Position topRight() const { return bottomLeft() + getSize() * 4; }

  // the radius of the circumscribed cirel
  double radius() const { return m_radius; }

  /**
   * @brief check the GraphObject overlaps with a specified rectangle
   * @param bl coordinates of the bottom left corner of the rectangle
   * @param tr coordinates of the top right corner of the rectangle
   * @return true if there is overlap
   */
  bool overlap(const Position& bl, const Position& tr) const {
    return (bl < topRight() && bottomLeft() < tr);
  }

  /**
   * @brief check the GraphObject overlaps with another object
   * @param other another object
   * @return true if there is overlap
   */
  bool overlap(const Actor& other) const {
    return overlap(other.bottomLeft(), other.topRight());
  }

  /**
   * @brief the distance between a position and the bottom left corner of the
   * object
   * @param pos the target position
   * @return the length
   */
  double distance(const Position& pos) const;

  /**
   * @brief the distance between 2 objects' bottom left corners
   * @param other the other object
   * @return the length
   */
  double distance(const Actor& other) const;

 protected:
  StudentWorld* world() { return m_world; }
  const StudentWorld* world() const { return m_world; }

  /**
   * @brief get the new position based on the current position, direction and
   * distance
   * @param dist the distance from the TunnelMan
   * @return the new corrdinates
   */
  Position directionalPosition(int dist, const Direction& dir) const;

 private:
  Actor() = delete;
  Actor(const Actor&) = delete;

  StudentWorld* m_world;  // pointer to the StudentWorld
  double m_radius;        // the radius of the circumscribed cirel
};

class Goodie : public Actor {
 public:
  enum class Pickable {
    player,    // pickable by a tunnel man
    protester  // pickable by a protester
  };

  Goodie(StudentWorld* world,
         int tid,
         const Position& pos,
         int ttl,
         bool visible);

  bool alive() const override { return m_ttl != 0; }
  void doSomething() override;

  /**
   * @brief the score contributed by the object when it is picked up
   */
  virtual int score() const = 0;

  virtual int sound() const { return SOUND_GOT_GOODIE; }

  /**
   * @brief who can pickup the object
   */
  virtual Pickable pickable() const { return Pickable::player; }

  /**
   * @brief check the distance from the TunnelMan, do something accordingly if
   * close
   */
  virtual void detectPlayer();
  /**
   * @brief set the object to dead
   */
  void die() { m_ttl = 0; }

  /**
   * @brief  decrease the life of the object by 1 tick if it is in temporary
   * state
   */
  void decreaseLife() {
    if (m_ttl > 0)
      --m_ttl;
  };

 private:
  int m_ttl;  // ticks to live, -1 means the object is stable
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
class Gold : public Goodie {
 public:
  Gold(StudentWorld* world,
       const Position& pos,
       const Pickable& pickable = Pickable::player);

  void doSomething() final;

  int score() const final { return 10; }
  Pickable pickable() const final { return m_pickable; }

 private:
  Pickable m_pickable;
};

/**
 * @brief class Sonar
 * 1. ImageID: TID_SONAR
 * 2. start off facing rightward
 * 3. visible: true
 * 4. pickable by TunnelMan
 * 5. temprary state, exists in max(100, 300 ¨C 10*current_level_number) ticks
 * 6. depth: 2
 * 7. size: 1.0
 */
class Sonar : public Goodie {
 public:
  explicit Sonar(StudentWorld* world);

  int score() const final { return 75; }
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
  int golds() const { return m_golds; }
  int health() const { return m_hitPoints * 10; }

  void pickupGoodie(Goodie& goodie);

  /**
   * @brief the TunnelMan drop a gold nugget when TAB key is pressed
   */
  void dropGold();

 private:
  int m_hitPoints = 10;
  int m_waterUnits = 5;
  int m_sonarCharges = 1;
  int m_golds = 0;

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
   * @brief the TunnelMan illuminates oil barrels and gold nuggets when Z or z
   * is pressed
   */
  void scanField();
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
              // check to see if there is any Earth in the 4 squares
              // immediately below it.
    waiting,  // If none of the 4 squares beneath the Boulder have any Earth,
              // then the Boulder must transition into a waiting state and
              // keeps for 30 ticks
    falling,  // If the Boulder is in a waiting state and 30 ticks have
              // elapsed, then it must transition into a falling state
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

  /**
   * @brief check the Boulder object hits any other boulders
   * @return true if overlap
   */
  bool hitBoulder();
};

/**
 * @brief class Squirt
 * 1. Image ID: TID_WATER_SPURT
 * 2. A Squirt object's initial position depends on the TunnelMan's position
 * and direction
 * 3. A Squirt objects's direction is the same as the TunnelMan's direction
 * 4. A Squirt object starts off with an initial distance 4 squares
 * 5. Image depth: 1
 * 6. Size: 1.0
 * 7. Visible: true
 */
class Squirt : public Actor {
 public:
  Squirt(StudentWorld* world, const Position& p, const Direction dir)
      : Actor(world, TID_WATER_SPURT, p, true, dir, 1){};

  bool alive() const final { return m_distance > 0; }

  void doSomething() final;

 private:
  int m_distance = 4;
};

/**
 * @brief class Barrel
 * 1. Image ID: TID_BARREL
 * 2. A Barrel object's initial x, y location is set by the caller
 * 3. A Barrel object starts off facing rightward
 * 4. Image depth: 2
 * 5. Size: 1.0
 * 6. Visible: false
 */
class Barrel : public Goodie {
 public:
  Barrel(StudentWorld* world, const Position& pos)
      : Goodie(world, TID_BARREL, pos, -1, false){};

  int score() const final { return 1000; }
  int sound() const final { return SOUND_FOUND_OIL; }
};

/**
 * @brief class WaterPool
 * 1. ImageID: TID_WATER_POOL
 * 2. location: x, y
 * 3. starts off facing right
 * 4. visible: true
 * 5. pickup-able by TunnelMan
 * 6. starts off temporary state, exists max(100, 300 ¨C
 * 10*current_level_number) ticks
 * 7. depth: 2
 * 8. size: 1.0
 */
class WaterPool : public Goodie {
 public:
  WaterPool(StudentWorld* world, const Position& pos);

  int score() const final { return 100; }
};

class Protester : public Actor {
 public:
  Protester(StudentWorld* world, int tid, int hitPoints)
      : Actor(world, tid, {60, 60}, true, Direction::left),
        m_hitPoints(hitPoints) {
    recalculateSteps();
  };

  bool alive() const final {
    // the protester is dead if it is in leave-the-oil-field state and reaches
    // the original position (60,60)
    return !(m_leaving && getX() == 60 && getY() == 60);
  }

  void annoyed(int points) override;

  void doSomething() override;

 protected:
  // move closer to the player if the TunnelMan is detected
  // return true if action been taken
  virtual bool chasePlayer() { return false; }

  // random walk without knowing where the TunnelMan is
  virtual void randomWalk();

  /**
   * @brief find a shortest path to the destination
   * @param path, the list of locations to the destination
   * @param target the destination
   * @param limit, the max steps allowed
   */
  void findWay(std::vector<Position>& path,
               const Position& target,
               int limit) const;

  void resetSteps() { m_stepsForward = 0; }

  // if the protester is in a straight horizontal or vertical line of sight to
  // the TunnelMan and more than 4 units away from the TunnelMan and no Earth
  // or Boulder blocking its path, get the direction to TunnelMan. otherwise
  // return Direction::none
  Direction straightToPlayer(const TunnelMan& player) const;

  // change the Protester direction to the new position
  // the new poistion must be on the same row or same colomn
  void turnTo(const Position& pos);

 private:
  bool m_leaving = false;  // the protester is leaving the oil field
  int m_hitPoints;
  int m_ttw;      // ticks to wait between moves, aka resting ticks
  int m_tts = 0;  // non-resting ticks to wait before shouting again
  int m_ttt =
      200;  // non-resting ticks to wait before making a perpendicular turn
  int m_stepsForward;  // number of squares the protester will move on its
                       // current direction before change direction

  // reset the ticks to wait to max(0, 3-current_level/4)
  void resetTtw();

  // go back the exit point (60,60)
  void exit();

  // check the protester is facing to the TunnelMan
  bool faceToPlayer(const TunnelMan& player) const;

  // annoy the TunnelMan
  void shout(TunnelMan& player);

  /**
   * @brief randomly pickup a new direction.
   */
  void newDirection();

  // check the Protester can move one step on the direction
  bool moveable(const Direction& dir) const;

  void getPath(
      std::vector<Position>& path,
      const std::array<std::array<short, VIEW_HEIGHT>, VIEW_WIDTH>& matrix,
      const Position& target) const;

  void recalculateSteps() {
    m_stepsForward = rand() % 53 + 8;  // a random number in [8, 60]
  }
};

/**
 * @brief class RegularProtester
 * 1. ImageID: TID_PROTESTER
 * 2. starts off facing left
 * 3. decide how many squares to move in the current direction in range [8,
 * 60]
 * 4. starts out with 5 hit-points
 * 5, starts out NOT in leave-the-oil-field state
 * 6. depth:0
 * 7. size: 1.0
 * 8. visible: true
 */
class RegularProtester : public Protester {
 public:
  explicit RegularProtester(StudentWorld* world)
      : Protester(world, TID_PROTESTER, 5){};

  void pickupGold() final;

 protected:
  // turn to the TunnelMan if the TunnelMan can be straight seen
  bool chasePlayer() final;
};

/**
 * @brief class HardcoreProtester
 * 1. ImageID: TID_HARD_CORE_PROTESTER
 * 2. starts off facing left
 * 3. decide how many squares to move in the current direction in range [8,
 * 60]
 * 4. starts out with 20 hit-points
 * 5, starts out NOT in leave-the-oil-field state
 * 6. depth:0
 * 7. size: 1.0
 * 8. visible: true
 */
class HardcoreProtester : public Protester {
 public:
  explicit HardcoreProtester(StudentWorld* world)
      : Protester(world, TID_HARD_CORE_PROTESTER, 20){};

  void pickupGold() final;

 protected:
  // Check the TunnelMan can be reached within 8 moves
  bool chasePlayer() final;
};

#endif  // ACTOR_H_
