#include "StudentWorld.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

using namespace std;

#define SQUARE(x) (x) * (x)

GameWorld* createStudentWorld(string assetDir) {
  return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and
// Actor.cpp

int StudentWorld::init() {
  m_player = std::make_unique<TunnelMan>(this);
  createEarth();

  // add boulders
  createBoulders();

  // add gold nuggets
  createGolds();

  // add oil barrels
  createBarrels();

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // Update the Game Status Line
  updateDisplayText();  // update the score/lives/level text at screen top
  // The term ¡°Actors¡± refers to all Protesters, the player, Goodies,
  // Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
  // Give each Actor a chance to do something
  m_player->doSomething();

  for (auto& actor : m_actors) {
    // ask each actor to do something (e.g. move)
    if (actor->alive()) {
      // ask each actor to do something (e.g. move)
      actor->doSomething();

      if (!m_player->alive())
        return GWSTATUS_PLAYER_DIED;
      if (levelCompleted()) {
        return GWSTATUS_FINISHED_LEVEL;
      }
    }
  }

  // Remove newly-dead actors after each tick
  removeDeadGameObjects();  // delete dead game objects
  // return the proper result
  if (!m_player->alive())
    return GWSTATUS_PLAYER_DIED;
  // If the player has collected all of the Barrels on the level, then
  // return the result that the player finished the level
  if (levelCompleted()) {
    playSound(GWSTATUS_FINISHED_LEVEL);
    return GWSTATUS_FINISHED_LEVEL;
  }
  // the player hasn¡¯t completed the current level and hasn¡¯t died
  // let them continue playing the current level

  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
  m_player = nullptr;
  m_actors.clear();
  m_earthMap.clear();
}

int StudentWorld::discover(const Position& p, int radius) {
  int barrels = 0;
  for (auto& actor : m_actors) {
    if (actor->getID() != TID_BARREL && actor->getID() != TID_GOLD) {
      continue;
    }

    if (SQUARE(p.x - actor->getX()) + SQUARE(p.y - actor->getY()) <
        SQUARE(radius)) {
      actor->setVisible(true);
      ++barrels;
    }
  }
  return barrels;
}

Position StudentWorld::generatePosition(int left,
                                        int right,
                                        int bottom,
                                        int top,
                                        int radius) const {
  Position p;
  do {
    p.x = rand() % (right - left + 1) + left;  // random number in [left, right]
    p.y =
        rand() % (top - bottom + 1) + bottom;  // random number in [bottom, top]
  } while (any_of(m_actors.begin(), m_actors.end(),
                  [&, p, radius](const unique_ptr<Actor>& actor) {
                    return (SQUARE(p.x - actor->getX()) +
                            SQUARE(p.y - actor->getY())) < SQUARE(radius);
                  }));

  return p;
}

/**
 * @brief create the Earth objects of the oil field from (0, 0) to (59, 63)
 * the Earth objects will cover the entire field except a mine shaft in the
 * middle, from (4, 30) to (59, 33)
 */
void StudentWorld::createEarth() {
  for (int row = 0; row < VIEW_HEIGHT; ++row) {
    m_earthMap.emplace_back();
    for (int col = 0; col < VIEW_WIDTH; ++col) {
      if (row < 60 && (row < 4 || col < 30 || col > 33)) {
        Position p{col, row};
        m_earthMap.back().emplace_back(make_unique<Earth>(this, p));
      } else {
        m_earthMap.back().emplace_back(nullptr);
      }
    }
  }
}

void StudentWorld::createBoulders() {
  int numBoulders = min(getLevel() / 2 + 2, 9U);
  // Boulders must be distributed between x=0,y=20 and x=60,y=56, inclusive (so
  // they have room to fall)
  for (int i = 0; i < numBoulders; ++i) {
    const auto p = generatePosition(0, 60, 20, 56, 6);
    // There must not be any Earth overlapping the 4x4 square region of each
    // Boulder, so we need to clear this Earth out when you place your Boulders
    // within the oil field
    checkEarth(p, {p.x + 4, p.y + 4}, true);
    m_actors.emplace_back(make_unique<Boulder>(this, p));
  }
}

void StudentWorld::createGolds() {
  int numGolds = max(5 - getLevel() / 2, 2U);
  // Nuggets and Oil Barrels must be distributed between x=0,y=0 and x=60,y=56
  // inclusive
  for (int i = 0; i < numGolds; ++i) {
    auto p = generatePosition(0, 60, 0, 56, 6);
    m_actors.emplace_back(make_unique<Gold>(this, p));
  }
}

void StudentWorld::createBarrels() {
  int numBarrels = min(2 + getLevel(), 21U);
  for (int i = 0; i < numBarrels; ++i) {
    const auto p = generatePosition(0, 60, 0, 56, 6);
    m_actors.emplace_back(make_unique<Barrel>(this, p));
  }
}

int StudentWorld::checkEarth(const Position& bottomLeft,
                             const Position& topRight,
                             bool clean) {
  int n = 0;
  for (int x = bottomLeft.x; x < topRight.x; ++x) {
    for (int y = bottomLeft.y; y < topRight.y; ++y) {
      if (!m_earthMap[y][x]) {
        continue;
      }

      ++n;
      if (clean) {
        m_earthMap[y][x] = nullptr;
      }
    }
  }
  return n;
}

bool StudentWorld::walkable(int x, int y) const {
  if (x < 0 || x > 60 || y < 0 || y > 60)
    return false;

  return none_of(m_actors.begin(), m_actors.end(),
                 [&, x, y](const unique_ptr<Actor>& actor) {
                   return (actor->getID() == TID_BOULDER) &&
                          actor->overlap({x, y}, {x + 4, y + 4});
                   // although the spec says the TunnelMan can't occupy a square
                   // that is less than or equal to a radius of 3 away from the
                   // center of any Boulder, the behavior of the example program
                   // seems that the TunnelMan can go anywhere in the field as
                   // long as there is no overlap on the 4*4 square of a Boulder
                 });
}

void StudentWorld::updateDisplayText() {
  /// Scr: 321000  Lvl: 52  Lives: 3  Hlth: 80%  Wtr: 20  Gld: 3   Sonar: 1  Oil
  /// Left: 2
  ///
  /// Each field must be exactly as wide as shown in the example above:
  ///   a.The Lvl field must be 2 digits long, with leading spaces(e.g., ¡°_1¡±,
  /// where _ represents a space).
  ///   b.The Lives field should be 1 digit long(e.g., ¡°2¡±).
  ///   c.The Hlth field should be 3 digits long and display the player¡¯s
  /// health percentage(not its hit - points !), with leading spaces, and be
  /// followed by a percent sign(e.g., ¡°_70 %¡±).
  ///   d.The Wtr field should be 2 digits long, with a leading space as
  /// required(e.g., ¡°_ 7¡±).
  ///   e.The Gld field should be 2 digits long, with a leading space as
  /// required(e.g., ¡°_ 3¡±).
  ///   f.The Oil Left field should be 2 digits long, with a leading space as
  /// required(e.g., ¡°_ 4¡±).
  ///   g.The Sonar field should be 2 digits long, with a leading space as
  /// required(e.g., ¡°_ 2¡±).
  ///   h.The Scr must be exactly 6 digits long, with leading zeros(e.g.,
  ///   003124).
  /// Each statistic must be separated from the last statistic by two spaces.
  ///  For example, between the ¡°000100¡± of the score and the ¡°L¡± in ¡°Level¡±
  ///  there must be exactly two spaces.

  int health = 100;

  int oil = 3;
  ostringstream oss;
  oss << "Scr: " << setw(6) << setfill('0') << getScore()
      << "  Lvl: " << setw(2) << setfill(' ') << getLevel()
      << "  Lives: " << setw(1) << getLives() << "  Hlth: " << setw(3) << health
      << "%  Wtr: " << setw(2) << m_player->waterUnits() << "  Gld: " << setw(2)
      << m_player->golds() << "  Sonar: " << setw(2) << m_player->sonarCharges()
      << "  Oil Left: " << setw(2) << oil;
  setGameStatText(oss.str());
}
