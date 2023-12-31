#include "Actor.h"
#include "GameConstants.h"
#include "GameController.h"
#include "StudentWorld.h"

#include <algorithm>
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

Position Actor::directionalPosition(int dist, const Direction& dir) const {
  switch (dir) {
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
  detectPlayer();
}

void Goodie::detectPlayer() {
  if (pickable() != Pickable::player) {
    return;
  }

  auto& player = world()->getPlayer();
  double dist = distance(player);
  if (dist <= 3) {
    // if the goodie is pickup-able by the TunnelMan and it is within a
    // radius of 3.0(<= 3.00 units away) from the TunnelMan, call TunnelMan's
    // pickupGoodie method
    player.pickupGoodie(*this);
  } else if (dist <= 4) {
    setVisible(true);
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
  if (world()->removeEarth(bottomLeft(), topRight())) {
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
  // If the TunnelMan��s hit-points reach zero or below due to being annoyed, the
  // TunnelMan must set its object��s state to dead, and then play an ��I give up��
  // sound effect : SOUND_PLAYER_GIVE_UP. Note : The StudentWorld class should
  // check the TunnelMan��s status during each tick and if he transitions to a
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
      .emplace_back(make_unique<Squirt>(
          world(), directionalPosition(4, getDirection()), getDirection()));
  Game().playSound(SOUND_PLAYER_SQUIRT);
}

void TunnelMan::pickupGoodie(Goodie& goodie) {
  // if the goodie is pickup-able by the TunnelMan and it is within a
  // radius of 3.0(<= 3.00 units away) from the TunnelMan, then the it
  // will

  // update inventory
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
    case TID_BARREL:
      break;
    default:
      return;
  }
  goodie.setVisible(true);                 // activate
  goodie.die();                            // set state to dead
  world()->playSound(goodie.sound());      // play sound
  world()->increaseScore(goodie.score());  // increase score
}

void TunnelMan::move(int key) {
  const auto dir = keyToDirection(key);
  if (getDirection() == dir) {  // same direction, move forward
    const auto target = directionalPosition(1, dir);
    if (!world()->validPosition(target)) {
      return;
    }

    if (world()->noBoulder(target)) {
      moveTo(target.x, target.y);
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
    world()->discover(TID_BARREL, 12);
    // search gold nuggets
    world()->discover(TID_GOLD, 12);
    --m_sonarCharges;
  }
}

void TunnelMan::dropGold() {
  if (m_golds > 0) {
    --m_golds;
    world()->getActors(TID_GOLD).emplace_back(
        make_unique<Gold>(world(), directionalPosition(4, getDirection()),
                          Gold::Pickable::protester));
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

bool Boulder::onEarth() const {
  // one row under the boulder
  const Position bottomLeft(getX(), getY() - 1);
  const Position topRight(getX() + 4, getY());
  return world()->hasEarth(bottomLeft, topRight);
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
      m_distance = 0;         // hit a protester, the squirt dies
      protester->annoyed(2);  // hurt a protester with 2 points
      if (protester->getID() == TID_PROTESTER) {
        world()->increaseScore(100);
      } else {
        world()->increaseScore(250);
      }
    }
  }

  if (!alive()) {
    return;
  }

  const auto p = directionalPosition(1, getDirection());
  const Position topRight{p.x + 4, p.y + 4};
  if (world()->hasEarth(p, topRight)) {
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
// Gold
Gold::Gold(StudentWorld* world, const Position& pos, const Pickable& pickable)
    : Goodie(world,
             TID_GOLD,
             pos,
             pickable == Pickable::player ? -1 : 100,
             pickable == Pickable::player ? false : true),
      m_pickable(pickable) {}

void Gold::doSomething() {
  if (!alive()) {
    return;
  }

  decreaseLife();

  detectPlayer();

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
    //    d. The Gold Nugget increases the player��s score by 25 points
    for (auto const& protester : world()->getActors(TID_PROTESTER)) {
      if (distance(*protester) <= 3) {
        die();

        protester->pickupGold();
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
void Protester::annoyed(int points) {
  if (m_leaving) {
    // the Protester can't be annoyed if leaving the field
    return;
  }

  m_hitPoints -= points;
  world()->playSound(SOUND_PROTESTER_ANNOYED);
  m_ttw = max(50U, 100 - world()->getLevel() * 10);
  if (m_hitPoints <= 0) {
    m_leaving = true;
    world()->playSound(SOUND_PROTESTER_GIVE_UP);
    m_ttw = 0;
  }
}

void Protester::doSomething() {
  if (!alive()) {
    return;
  }

  if (m_ttw-- > 0) {
    return;
  }

  resetTtw();
  --m_tts;  // decrease non-resting ticks for shouting
  --m_ttt;  // decrease non-resting ticks for turning

  if (m_leaving) {
    exit();
    return;
  }

  TunnelMan& player = world()->getPlayer();
  double awayFromPlayer = distance(player);
  if (awayFromPlayer <= 4 && faceToPlayer(player) && m_tts <= 0) {
    shout(player);
    return;
  }

  if (chasePlayer()) {
    return;
  }

  randomWalk();
}

void Protester::randomWalk() {
  if (m_stepsForward > 0 && !moveable(getDirection())) {
    // the Protester is blocked by either Earth or Boulder
    m_stepsForward = 0;
    return;
  }

  // decide the direction of the next step and move
  newDirection();
  auto target = directionalPosition(1, getDirection());
  if (world()->validPosition(target)) {
    moveTo(target.x, target.y);
  } else {
    resetSteps();
  }
  --m_stepsForward;
}

void Protester::resetTtw() {
  m_ttw = max(0, 3 - static_cast<int>(world()->getLevel()) / 4);
}

void Protester::turnTo(const Position& pos) {
  if (pos.x > getX())
    setDirection(Direction::right);
  else if (pos.x < getX())
    setDirection(Direction::left);
  else if (pos.y > getY())
    setDirection(Direction::up);
  else if (pos.y < getY())
    setDirection(Direction::down);
}

void Protester::exit() {
  vector<Position> path;
  findWay(path, Position(60, 60), VIEW_HEIGHT * VIEW_WIDTH);

  if (!path.empty()) {
    auto const& next = path.front();

    turnTo(next);
    moveTo(next.x, next.y);
  }
}

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
  m_tts = 15;
}

void Protester::findWay(vector<Position>& path,
                        const Position& target,
                        int limit) const {
  // store the distance to each point, -1 means unvisited
  array<array<short, VIEW_HEIGHT>, VIEW_WIDTH> matrix;
  for (int x = 0; x < VIEW_WIDTH; ++x)
    for (int y = 0; y < VIEW_HEIGHT; ++y)
      matrix[x][y] = -1;
  matrix[getX()][getY()] = 0;  // start point is 0
  queue<Position> visited;
  visited.push(bottomLeft());

  while (!visited.empty()) {
    auto pos = visited.front();  // get the first element from the queue
    visited.pop();

    if (pos == target) {
      // get the path from the matrix reversely, terminate the loop when reaches
      // the destination
      getPath(path, matrix, target);
      return;
    }

    // get the distance of the current point
    short current = matrix[pos.x][pos.y];
    if (current > limit) {
      return;
    }
    for (int i = 0; i < 4; ++i) {
      auto newPos = Position(pos.x + (i - 2) % 2, pos.y + (i - 1) % 2);
      if (world()->validPosition(newPos) && matrix[newPos.x][newPos.y] < 0 &&
          !world()->hasEarth(newPos, newPos + 4) &&
          world()->noBoulder(newPos)) {
        visited.push(newPos);
        matrix[newPos.x][newPos.y] = current + 1;
      }
    }
  }
}

GraphObject::Direction Protester::straightToPlayer(
    const TunnelMan& player) const {
  if (distance(player) < 4)
    return Direction::none;

  auto bl = Position(min(getX(), player.getX()), min(getY(), player.getY()));
  auto tr =
      Position(max(getX(), player.getX()) + 4, max(getY(), player.getY()) + 4);
  if (world()->hasEarth(bl, tr)) {
    return Direction::none;
  }
  auto& boulders = world()->getActors(TID_BOULDER);
  if (any_of(boulders.begin(), boulders.end(),
             [&, bl, tr](unique_ptr<Actor> const& actor) {
               return actor->overlap(bl, tr);
             })) {
    // any boulder on the way
    return Direction::none;
  }

  if (getX() == player.getX()) {
    return getY() < player.getY() ? Direction::up : Direction::down;
  }

  if (getY() == player.getY()) {
    return getX() < player.getX() ? Direction::right : Direction::left;
  }

  return Direction::none;
}

void Protester::newDirection() {
  vector<Direction> choices;

  if (m_ttt <= 0) {
    // The Protester hasn��t made a perpendicular turn in the last 200
    // nonresting ticks
    if (getDirection() <= Direction::down) {
      choices.push_back(Direction::left);
      choices.push_back(Direction::right);
    } else {
      choices.push_back(Direction::up);
      choices.push_back(Direction::down);
    }
  } else if (m_stepsForward <= 0) {
    // If the Regular Protester has finished walking m_stepsForward steps in its
    // currently-selected direction
    choices.push_back(Direction::up);
    choices.push_back(Direction::down);
    choices.push_back(Direction::left);
    choices.push_back(Direction::right);
  }

  // remove invalid directions
  choices.erase(remove_if(choices.begin(), choices.end(),
                          [this](const auto& dir) { return !moveable(dir); }),
                choices.end());
  if (choices.empty()) {
    // if not the time to turn, go forward
    return;
  }

  // randomly turn to a valid direction
  const auto dir = choices[rand() % choices.size()];
  if (getDirection() <= Direction::down && dir > Direction::down ||
      getDirection() > Direction::down && dir <= Direction::down) {
    m_ttt = 200;  // perpendicular turn
  }
  setDirection(dir);
  recalculateSteps();
}

bool Protester::moveable(const Direction& dir) const {
  const auto target = directionalPosition(1, dir);
  return (world()->validPosition(target) &&
          !world()->hasEarth(target, target + 4) &&
          world()->noBoulder(target));  // no earth no boulder in front
}

void Protester::getPath(
    vector<Position>& path,
    const std::array<std::array<short, VIEW_HEIGHT>, VIEW_WIDTH>& matrix,
    const Position& target) const {
  if (target == bottomLeft()) {
    return;  // origin == dest
  }

  for (int i = 0; i < 4; ++i) {
    short current = matrix[target.x][target.y];
    if (current < 0) {
      // something wrong, we traveral back to an unvisited location
      return;
    }

    int prevX = target.x + (i - 2) % 2;  // 0, -1, 0, 1
    int prevY = target.y + (i - 1) % 2;  // -1, 0, 1, 0
    if (matrix[prevX][prevY] + 1 == matrix[target.x][target.y]) {
      getPath(path, matrix, Position(prevX, prevY));
      path.emplace_back(target);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// RegularProtester
bool RegularProtester::chasePlayer() {
  // if the Protester:
  //    a.Is in a straight horizontal or vertical line of sight to the
  //    TunnelMan(even if the Regular Protester isn��t currently facing the
  //    TunnelMan), and
  //    b.Is more than 4 units away from the TunnelMan, and
  //    c.Could actually move the entire way to the TunnelMan with no Earth or
  //    Boulders blocking its path  const auto dir = straightToPlayer(player);
  auto dir = straightToPlayer(world()->getPlayer());
  if (dir != Direction::none) {
    setDirection(dir);  // turn to the player
    const auto target = directionalPosition(1, dir);
    moveTo(target.x, target.y);  // move 1 step forward to the player
    resetSteps();  // set its numSquaresToMoveInCurrentDirection value to
                   // zero, forcing it to pick a new direction/distance to
                   // move during its next non-resting tick
    return true;
  }

  return false;  // the player is not detected
}

void RegularProtester::pickupGold() {
  world()->playSound(SOUND_PROTESTER_FOUND_GOLD);
  world()->increaseScore(25);
}

//////////////////////////////////////////////////////////////////////////////
// HardcoreProtester
bool HardcoreProtester::chasePlayer() {
  const auto& player = world()->getPlayer();
  vector<Position> path;

  // check the TunnelMan is within 8 steps reachable position
  // if so, move forward to it
  findWay(path, player.bottomLeft(), 8);
  if (!path.empty()) {
    auto const& next = path.front();
    turnTo(next);
    moveTo(next.x, next.y);
    resetSteps();
    return true;
  }

  // if the TunnelMan can be straight seen and no block
  // go to that direction
  auto dir = straightToPlayer(world()->getPlayer());
  if (dir != Direction::none) {
    setDirection(dir);  // turn to the player
    const auto target = directionalPosition(1, dir);
    moveTo(target.x, target.y);  // move 1 step forward to the player
    resetSteps();  // set its numSquaresToMoveInCurrentDirection value to
                   // zero, forcing it to pick a new direction/distance to
                   // move during its next non-resting tick
    return true;
  }

  return false;
}

void HardcoreProtester::pickupGold() {
  world()->playSound(SOUND_PROTESTER_FOUND_GOLD);
  world()->increaseScore(50);
}