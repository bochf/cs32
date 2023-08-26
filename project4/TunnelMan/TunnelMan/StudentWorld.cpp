#include "StudentWorld.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

using namespace std;

GameWorld* createStudentWorld(string assetDir) {
  return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and
// Actor.cpp

int StudentWorld::init() {
  m_player = std::make_unique<TunnelMan>(this);
  m_actors[TID_BOULDER] = ACTORS();
  m_actors[TID_BARREL] = ACTORS();
  m_actors[TID_GOLD] = ACTORS();
  m_actors[TID_PROTESTER] = ACTORS();
  m_actors[TID_SONAR] = ACTORS();
  m_actors[TID_WATER_POOL] = ACTORS();
  m_actors[TID_WATER_SPURT] = ACTORS();
  m_pp.reset(getLevel());
  m_goodieChance = getLevel() * 25 + 300;

  createEarth();

  // add boulders
  createBoulders();

  // add gold nuggets
  createGolds();

  // add oil barrels
  createBarrels();

  addProtester();

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // Update the Game Status Line
  updateDisplayText();  // update the score/lives/level text at screen top

  --m_pp.m_wait;  // update the new protester waiting time

  m_player->doSomething();

  for (const auto& actors : m_actors) {
    for (auto& actor : actors.second) {
      if (!actor->alive()) {
        continue;
      }
      // ask each actor to do something (e.g. move)
      actor->doSomething();

      if (!m_player->alive()) {
        playSound(SOUND_PLAYER_GIVE_UP);
        return GWSTATUS_PLAYER_DIED;
      }
      if (levelCompleted()) {
        return GWSTATUS_FINISHED_LEVEL;
      }
    }
  }

  // Add new objects
  addNewObjects();

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

  for_each(m_earthMap.begin(), m_earthMap.end(), [](auto& row) {
    for_each(row.begin(), row.end(), [](auto& earth) { earth = nullptr; });
  });
}

int StudentWorld::discover(int tid, int radius) {
  int num = 0;
  const auto& pair = m_actors.find(tid);
  if (pair == m_actors.end()) {
    return 0;
  }

  for (const auto& hiddenObj : pair->second) {
    if (hiddenObj->distance(*m_player) < radius) {
      hiddenObj->setVisible(true);
      ++num;
    }
  }
  return num;
}

Position StudentWorld::generatePosition(int left,
                                        int right,
                                        int bottom,
                                        int top,
                                        int radius) const {
  Position p(0, 0);
  while (true) {
    bool valid = true;

    // generate a random coordinates in range [left, right] and [bottom, top]
    p.x = rand() % (right - left + 1) + left;
    p.y = rand() % (top - bottom + 1) + bottom;

    // check it sits in the range of other objects
    auto tooClose = [&, p, radius](const unique_ptr<Actor>& actor) {
      return (actor->distance(p) < radius);
    };
    for (const auto& pair : m_actors) {
      valid &= none_of(pair.second.begin(), pair.second.end(), tooClose);
      if (!valid)
        continue;
    }
    if (valid)
      break;
  }
  return p;
}

/**
 * @brief create the Earth objects of the oil field from (0, 0) to (59, 63)
 * the Earth objects will cover the entire field except a mine shaft in the
 * middle, from (4, 30) to (59, 33)
 */
void StudentWorld::createEarth() {
  for (int row = 0; row < VIEW_HEIGHT; ++row) {
    for (int col = 0; col < VIEW_WIDTH; ++col) {
      m_earthMap[row][col] = (row < 60 && (row < 4 || col < 30 || col > 33))
                                 ? make_unique<Earth>(this, Position(col, row))
                                 : nullptr;
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
    removeEarth(p, p + 4);
    m_actors[TID_BOULDER].emplace_back(make_unique<Boulder>(this, p));
  }
}

void StudentWorld::createGolds() {
  int numGolds = max(5 - getLevel() / 2, 2U);
  // Nuggets and Oil Barrels must be distributed between x=0,y=0 and x=60,y=56
  // inclusive
  for (int i = 0; i < numGolds; ++i) {
    auto p = generatePosition(0, 60, 0, 56, 6);
    m_actors[TID_GOLD].emplace_back(make_unique<Gold>(this, p));
  }
}

void StudentWorld::createBarrels() {
  int numBarrels = min(2 + getLevel(), 21U);
  for (int i = 0; i < numBarrels; ++i) {
    const auto p = generatePosition(0, 60, 0, 56, 6);
    m_actors[TID_BARREL].emplace_back(make_unique<Barrel>(this, p));
  }
}

bool StudentWorld::removeEarth(const Position& bottomLeft,
                               const Position& topRight) {
  bool hasEarth = false;
  for (int x = bottomLeft.x; x < topRight.x; ++x) {
    for (int y = bottomLeft.y; y < topRight.y; ++y) {
      if (m_earthMap[y][x]) {
        hasEarth = true;
        m_earthMap[y][x] = nullptr;
      }
    }
  }

  if (hasEarth) {
    playSound(SOUND_DIG);
  }

  return hasEarth;
}

bool StudentWorld::hasEarth(const Position& bottomLeft,
                            const Position& topRight) const {
  for (int x = bottomLeft.x; x < topRight.x; ++x) {
    for (int y = bottomLeft.y; y < topRight.y; ++y) {
      if (m_earthMap[y][x]) {
        return true;
      }
    }
  }
  return false;
}

void StudentWorld::onBoulderFall(const Boulder& boulder) {
  if (m_player->overlap(boulder)) {
    m_player->annoyed(100);
  }
  for (const auto& protester : m_actors.at(TID_PROTESTER)) {
    if (protester->overlap(boulder)) {
      protester->annoyed(100);
      // add 500 score to the player
      increaseScore(500);
    }
  }
}

bool StudentWorld::noBoulder(const Position& pos) const {
  const auto& boulders = m_actors.at(TID_BOULDER);

  return none_of(boulders.begin(), boulders.end(),
                 [&, pos](const unique_ptr<Actor>& boulder) {
                   return boulder->overlap(pos, pos + 4);
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

  ostringstream oss;
  oss << "Scr: " << setw(6) << setfill('0') << getScore()
      << "  Lvl: " << setw(2) << setfill(' ') << getLevel()
      << "  Lives: " << setw(1) << getLives() << "  Hlth: " << setw(3)
      << m_player->health() << "%  Wtr: " << setw(2) << m_player->waterUnits()
      << "  Gld: " << setw(2) << m_player->golds() << "  Sonar: " << setw(2)
      << m_player->sonarCharges() << "  Oil Left: " << setw(2)
      << m_actors[TID_BARREL].size();
  setGameStatText(oss.str());
}

bool StudentWorld::levelCompleted() const {
  return m_actors.at(TID_BARREL).empty();
}

void StudentWorld::addNewObjects() {
  addProtester();
  if (rand() % m_goodieChance == 0) {
    if (rand() % 5 == 0)
      addSonar();
    else
      addWaterPool();
  }
}

void StudentWorld::addProtester() {
  if (protesters() < m_pp.m_max && m_pp.m_wait <= 0) {
    m_pp.resetWaitTime();
    int tmp = rand() % 100;
    if (tmp < m_pp.m_hardcoreProbability) {
      // add a hardcore protester
      m_actors[TID_PROTESTER].emplace_back(
          make_unique<HardcoreProtester>(this));
    } else {
      // add a regular protester
      m_actors[TID_PROTESTER].emplace_back(make_unique<RegularProtester>(this));
    }
  }
}

void StudentWorld::addWaterPool() {
  // add a water pool
  Position p(0, 0);
  while (true) {
    p.x = rand() % 60;
    p.y = rand() % 60;

    // Water may only be added to a location if the entire 4x4 grid at that
    // location is free of Earth.
    if (!hasEarth(p, p + 4)) {
      m_actors[TID_WATER_POOL].emplace_back(make_unique<WaterPool>(this, p));
      return;
    }
  }
}

void StudentWorld::addSonar() {
  m_actors[TID_SONAR].emplace_back(make_unique<Sonar>(this));
}

void StudentWorld::removeDeadGameObjects() {
  for (auto& actors : m_actors) {
    actors.second.erase(
        remove_if(actors.second.begin(), actors.second.end(),
                  [](const auto& actor) { return (!actor->alive()); }),
        actors.second.end());
  }
}