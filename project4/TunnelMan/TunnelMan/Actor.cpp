#include "Actor.h"
#include "GameConstants.h"
#include "GameController.h"
#include "StudentWorld.h"

#include <memory>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Utilities
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

double len(const Position& p1, const Position& p2) {
  return sqrt(SQUARE(p1.x - p2.x) + SQUARE(p1.y - p2.y));
};

//////////////////////////////////////////////////////////////////////////////
// Actor
Actor::Actor(StudentWorld* world,
             int tid,
             const Position& pos,
             bool visible,
             Direction dir,
             unsigned int depth,
             double size)
    : GraphObject(tid, pos.x, pos.y, dir, size, depth),
      m_world(world),
      m_radius(getSize() * 2 * sqrt(2)) {
  setVisible(visible);
}

Actor::~Actor() {
  m_world = nullptr;
}

double Actor::distance(const Position& pos) const {
  return len(bottomLeft(), pos);
}

double Actor::distance(const Actor& other) const {
  return distance(other.bottomLeft());
}

bool Actor::inCircle(const Position& pos, double r) const {
  Position center(bottomLeft() + getSize() * 2);
  return len(center, pos) <= r;
}

bool Actor::inCircle(const Actor& other, double r) const {
  Position c1(bottomLeft() + getSize() * 2);
  Position c2(other.bottomLeft() + other.getSize() * 2);

  return len(c1, c2) > other.radius() + r;
}

Position Actor::directionalPosition(int dist) const {
  switch (getDirection()) {
    case Direction::left:
      return Position(getX() - dist, getY());
    case right:
      return Position(getX() + dist, getY());
    case up:
      return Position(getX(), getY() + dist);
    case down:
      return Position(getX(), getY() - dist);
    default:
      return Position(getX(), getY());
  }
}

//////////////////////////////////////////////////////////////////////////////
// Goodie
Goodie::Goodie(StudentWorld* world,
               int tid,
               const Position& pos,
               int ttl,
               bool visible)
    : Actor(world, tid, pos, visible, Direction::right, 2, 1.0), m_ttl(ttl) {}

void Goodie::doSomething() {
  decreaseLife();

  auto& player = world()->getPlayer();
  if (distance(player) <= 3 && pickable() == Pickable::player) {
    // if the goodie is pickup-able by the TunnelMan and it is within a
    // radius of 3.0(<= 3.00 units away) from the TunnelMan, then the it
    // will activate, and:
    //  a. set its state to dead
    //
    //  b.play a sound effect to indicate that the TunnelMan picked up the
    //  Goodie : SOUND_GOT_GOODIE.
    //
    //  c.increases the player¡¯s score by some points
    //
    //  d. update player's inventory.
    player.pickGoodie(*this);
  }
}

//////////////////////////////////////////////////////////////////////////////
// TunnelMan
void TunnelMan::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  // if overlap any earth object, dig it
  if (world()->checkEarth(bottomLeft(), topRight(), true)) {
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
        scanField();
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

  --m_waterUnits;
  world()
      ->getActors(TID_WATER_SPURT)
      .emplace_back(
          make_unique<Squirt>(world(), directionalPosition(4), getDirection()));
  Game().playSound(SOUND_PLAYER_SQUIRT);
}

void TunnelMan::pickGoodie(Goodie& goodie) {
  switch (goodie.getID()) {
    case TID_WATER_POOL:
      m_waterUnits += 5;
      break;
    case TID_SONAR:
      ++m_sonarCharges;
      break;
    case TID_GOLD:
      ++m_golds;
      break;
    default:
      return;
  }
  goodie.setVisible(true);
  goodie.die();
  world()->playSound(SOUND_GOT_GOODIE);
  world()->increaseScore(goodie.score());
}

void TunnelMan::move(int key) {
  const auto dir = keyToDirection(key);
  if (getDirection() == dir) {  // same direction, move forward
    const auto p = directionalPosition(1);
    if (world()->walkable(p)) {
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

void TunnelMan::scanField() {
  if (m_sonarCharges > 0) {
    // search oil barrels
    world()->discover(TID_BARREL, 64);
    // search gold nuggets
    world()->discover(TID_GOLD, 64);
    --m_sonarCharges;
  }
}

void TunnelMan::dropGold() {
  if (m_golds > 0) {
    --m_golds;
    world()->getActors(TID_GOLD).emplace_back(make_unique<Gold>(
        world(), directionalPosition(4), Gold::Pickable::protester));
  }
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
      if (onEarth() || hitBoulder()) {
        m_state = State::dead;
      } else {
        moveTo(getX(), getY() - 1);
        world()->onBoulderFall(*this);
      }
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
  // one row under the boulder
  const Position bottomLeft(getX(), getY() - 1);
  const Position topRight(getX() + 4, getY());
  return world()->checkEarth(bottomLeft, topRight, false) > 0;
}

bool Boulder::hitBoulder() {
  for (const auto& boulder : world()->getActors(TID_BOULDER)) {
    if (bottomLeft() == boulder->bottomLeft()) {
      // skip self
      continue;
    }
    if (boulder->overlap(*this)) {
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
// Squirt
void Squirt::doSomething() {
  if (!alive()) {
    return;
  }

  // shoot a protester
  for (auto const& protester : world()->getActors(TID_PROTESTER)) {
    if (protester->overlap(*this)) {
      protester->annoyed(2);  // hurt a protester with 2 points
      m_distance = 0;         // hit a protester, the squirt dies
    }
  }

  if (!alive()) {
    return;
  }

  const auto p = directionalPosition(1);
  const Position topRight{p.x + 4, p.y + 4};
  if (world()->checkEarth(p, topRight, false) > 0) {
    m_distance = 0;  // the squirt hits on earth, dies
    return;
  }
  for (auto const& boulder : world()->getActors(TID_BOULDER)) {
    if (boulder->overlap(p, topRight)) {
      m_distance = 0;  // the squirt hits a boulder, dies
      return;
    }
  }

  // move forward if nothing block
  moveTo(p.x, p.y);
  --m_distance;
}

//////////////////////////////////////////////////////////////////////////////
// Barrel
void Barrel::doSomething() {
  if (!alive()) {
    return;
  }

  double awayFromPlayer = distance(world()->getPlayer());
  if (!isVisible() && (awayFromPlayer <= 4)) {
    // if the barrel is not currently visible, it can be discovered by a
    // TunnelMan if the TunnelMan is within a radius of 4.0 of it(<= 4.00 units
    // away)
    setVisible(true);
    return;
  }
  if (awayFromPlayer <= 3) {
    // if the Barrel is within a radius of 3.0(<= 3.00 units away) from the
    // TunnelMan, then the Barrel will activate, and:
    //  a. The Barrel must set its state to dead
    //
    //  b. The Barrel must play a sound effect to indicate that the player
    //  picked up the Goodie : SOUND_FOUND_OIL.
    //
    //  c. The Barrel must increase the player¡¯s score by 1000 points.
    //
    //  d. If necessary, the Barrel may inform the StudentWorld object that it
    //  was picked up. Once all Barrels on the level have been picked up, the
    //  player finishes the level and may advance to the next level.
    setVisible(true);
    m_alive = false;
    world()->playSound(SOUND_FOUND_OIL);
    world()->increaseScore(1000);
  }
}

//////////////////////////////////////////////////////////////////////////////
// Gold
Gold::Gold(StudentWorld* world, const Position& pos, const Pickable& pickable)
    : Goodie(world,
             TID_GOLD,
             pos,
             pickable == Pickable::player ? -1 : 100,
             pickable == Pickable::player ? false : true),
      m_pickable(pickable) {}

int Gold::score() const {
  return m_pickable == Pickable::player ? 10 : 25;
}

void Gold::doSomething() {
  if (!alive()) {
    return;
  }

  decreaseLife();

  TunnelMan& player = world()->getPlayer();
  double awayFromPlayer = distance(player);
  if (!isVisible() && (awayFromPlayer <= 4)) {
    //  if the Gold Nugget is not currently visible AND the TunnelMan is within
    //  a radius of 4.0 of it(<= 4.00 units away), then the Gold Nugget must
    //  make itself visible
    setVisible(true);
    return;
  }

  if (awayFromPlayer <= 3 && pickable() == Pickable::player) {
    // if the Gold Nugget is pickup-able by the TunnelMan and it is within a
    // radius of 3.0(<= 3.00 units away) from the TunnelMan, then the Gold
    // Nugget will activate, and:
    //  a.The Gold Nugget must set its state to dead
    //
    //  b.The Gold Nugget must play a sound effect to indicate that the
    //     TunnelMan picked up the Goodie : SOUND_GOT_GOODIE.
    //
    //  c.The Gold Nugget increases the player¡¯s score by 10 points
    //
    //  d.The Gold Nugget must tell the TunnelMan object that it justreceived
    //     a new Nugget so it can update its inventory.
    setVisible(true);
    player.pickGoodie(*this);
  }

  if (pickable() == Pickable::protester) {
    // if the Gold Nugget is pickup-able by Protesters and it is within a radius
    // of 3.0(<= 3.00 units away) from a Protester, then the Gold Nugget will
    // activate, and:
    //
    //    a.The Gold Nugget must set its state to dead
    //
    //    b. The Gold Nugget must play a sound effect to indicate that the
    //    Protester picked it up: SOUND_PROTESTER_FOUND_GOLD.
    //
    //    c. The Gold Nugget must tell the Protester object that it just
    //    received a new Nugget so it can react appropriately
    //
    //    d. The Gold Nugget increases the player¡¯s score by 25 points
    for (auto const& protester : world()->getActors(TID_PROTESTER)) {
      if (distance(*protester) <= 3) {
        die();
        world()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        world()->increaseScore(25);
        protester->pickGold();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// Sonar
Sonar::Sonar(StudentWorld* world)
    : Goodie(world,
             TID_SONAR,
             Position(0, 60),
             max(100U, 300 - 10 * world->getLevel()),
             true) {}

//////////////////////////////////////////////////////////////////////////////
// WaterPool
WaterPool::WaterPool(StudentWorld* world, const Position& pos)
    : Goodie(world,
             TID_WATER_POOL,
             pos,
             max(100U, 300 - 10 * world->getLevel()),
             true) {}

//////////////////////////////////////////////////////////////////////////////
// Protester
void Protester::doSomething() {
  if (!alive()) {
    return;
  }

  if (m_ttw-- > 0) {
    return;
  }

  resetTtw();

  if (m_leaving) {
    // TODO leave the oil field
    exit();
    return;
  }

  TunnelMan& player = world()->getPlayer();
  double awayFromPlayer = distance(player);
  if (awayFromPlayer <= 4 && faceToPlayer(player) && m_silent <= 0) {
    shout(player);
    return;
  }
}

void Protester::resetTtw() {
  m_ttw = max(0, 3 - static_cast<int>(world()->getLevel()) / 4);
}

void Protester::exit() {}

bool Protester::faceToPlayer(const TunnelMan& player) const {
  switch (getDirection()) {
    case Direction::down:
      return getY() > player.getY();
    case Direction::left:
      return getX() > player.getX();
    case Direction::right:
      return getX() < player.getX();
    case Direction::up:
      return getY() < player.getY();
    default:
      return false;
  }
}

void Protester::shout(TunnelMan& player) {
  world()->playSound(SOUND_PROTESTER_YELL);
  player.annoyed(2);
  m_silent = 15;
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
