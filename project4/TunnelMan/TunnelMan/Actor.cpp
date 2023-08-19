#include "Actor.h"
#include "GameConstants.h"
#include "GameController.h"
#include "StudentWorld.h"

#include <memory>
#include <vector>

using namespace std;

/**
 * @brief translate the user pressed key to the direction
 * @param key the input key, e.g. left, right, up, down
 * @return the corresponding direction
 */
GraphObject::Direction keyToDirection(int key) {
  switch (key) {
    case KEY_PRESS_LEFT:
      return GraphObject::Direction::left;
    case KEY_PRESS_RIGHT:
      return GraphObject::Direction::right;
    case KEY_PRESS_UP:
      return GraphObject::Direction::up;
    case KEY_PRESS_DOWN:
      return GraphObject::Direction::down;
    default:
      return GraphObject::Direction::none;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Actor
Actor::Actor(StudentWorld* world,
             int imageID,
             const Position& p,
             bool visible,
             Direction dir,
             unsigned int depth,
             double size)
    : GraphObject(imageID, p.x, p.y, dir, size, depth), m_world(world) {
  setVisible(visible);
}

Actor::~Actor() {
  m_world = nullptr;
}

/*
void Actor::occupies(vector<Position>& area) const {
  int x = getX();
  int y = getY();
  const int size = roundAwayFromZero(getSize() * 4);
  area.clear();
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      area.push_back({x + i, y + j});
    }
  }
}
*/

bool Actor::overlap(const Position& bottomLeft,
                    const Position& topRight) const {
  return (bottomLeft.x < getX() + getSize() * 4 && getX() < topRight.x &&
          bottomLeft.y < getY() + getSize() * 4 && getY() < topRight.y);
}

bool Actor::overlap(const GraphObject& other) const {
  const Position bottomLeft{other.getX(), other.getY()};
  const Position topRight{other.getX() + other.getSize() * 4,
                          other.getY() + other.getSize() * 4};
  return overlap(bottomLeft, topRight);
}

//////////////////////////////////////////////////////////////////////////////
// TunnelMan
void TunnelMan::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  // if overlap any earth object, dig it
  const Position bottomLeft{getX(), getY()};
  const Position topRight{getX() + 4, getY() + 4};
  if (world()->checkEarth(bottomLeft, topRight, true)) {
    Game().playSound(SOUND_DIG);
    return;
  }

  // check user input
  int key;
  if (world()->getKey(key)) {
    switch (key) {
      case KEY_PRESS_ESCAPE:  // abort current level
        quit();
        break;
      case KEY_PRESS_SPACE:  // fire a squirt
        fire();
        break;
      case KEY_PRESS_LEFT:
      case KEY_PRESS_RIGHT:
      case KEY_PRESS_UP:
      case KEY_PRESS_DOWN:
        move(key);
        break;
      case 'Z':
      case 'z':
        scanOil();
        break;
      case KEY_PRESS_TAB:
        dropGold();
        break;
      default:
        break;
    }
  }
}

void TunnelMan::annoyed(int deduction) {
  // When the TunnelMan is annoyed (i.e., shouted at by a Protester), its
  // hit-points should be decremented by the appropriate amount(e.g., 2 points)
  //
  // If the TunnelMan¡¯s hit-points reach zero or below due to being annoyed, the
  // TunnelMan must set its object¡¯s state to dead, and then play an ¡°I give up¡±
  // sound effect : SOUND_PLAYER_GIVE_UP. Note : The StudentWorld class should
  // check the TunnelMan¡¯s status during each tick and if he transitions to a
  // dead state, it should return the appropriate value indicating that the
  // player lost a life.
  m_hitPoints -= deduction;
}

void TunnelMan::fire() {
  if (m_waterUnits <= 0) {
    return;
  }

  const auto p = newPosition(getDirection(), 4);
  world()->addActor(make_unique<Squirt>(world(), p, getDirection()));
  Game().playSound(SOUND_PLAYER_SQUIRT);
}

void TunnelMan::move(int key) {
  const auto dir = keyToDirection(key);
  if (getDirection() == dir) {  // same direction, move forward
    const auto p = newPosition(dir, 1);
    if (world()->walkable(p.x, p.y)) {
      moveTo(p.x, p.y);
    }
  } else {
    // different direction, turn
    setDirection(dir);
  }
}

void TunnelMan::quit() {
  m_hitPoints = 0;  // dead
}

void TunnelMan::scanOil() {
  if (m_sonarCharges > 0) {
    // search Barrel objects
    world()->discover({getX(), getY()}, 12);
    --m_sonarCharges;
  }
}

void TunnelMan::dropGold() {
  if (!m_golds.empty()) {
    m_golds.pop_back();
  }
}

Position TunnelMan::newPosition(const Direction& dir, int dist) const {
  int x = getX();
  int y = getY();
  switch (dir) {
    case Direction::left:
      x = getX() - dist;
      break;
    case right:
      x = getX() + dist;
      break;
    case up:
      y = getY() + dist;
      break;
    case down:
      y = getY() - dist;
      break;
    default:
      break;
  }
  return {x, y};
}

//////////////////////////////////////////////////////////////////////////////
// Boulder
void Boulder::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  switch (m_state) {
    case State::stable:
      // check there is any earth object under the boulder
      if (!onEarth()) {
        m_state = State::waiting;
      }
      break;
    case State::falling:
      if (onEarth() || !world()->walkable(getX(), getY() - 1)) {
        m_state = State::dead;
      }
      moveTo(getX(), getY() - 1);
      break;
    case State::waiting:
      if (m_waitTicks < 30) {
        ++m_waitTicks;
      } else {
        Game().playSound(SOUND_FALLING_ROCK);
        m_state = State::falling;
      }
      break;
    case State::dead:
      break;
    default:
      break;
  }
}

bool Boulder::onEarth() {
  const Position bottomLeft{getX(), getY() - 1};
  const Position topRight{getX() + 4, getY()};
  return world()->checkEarth(bottomLeft, topRight, false) > 0;
}

//////////////////////////////////////////////////////////////////////////////
// Squirt
void Squirt::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// Barrel
void Barrel::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// Gold
void Gold::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// Sonar
void Sonar::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// WaterPool
void WaterPool::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// RegularProtester
void RegularProtester::doSomething() {
  // TODO
}

//////////////////////////////////////////////////////////////////////////////
// HardcoreProtester
void HardcoreProtester::doSomething() {
  // TODO
}
