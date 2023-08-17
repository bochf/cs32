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
  m_numBoulders = min(getLevel() / 2 + 2, 9U);
  m_numGolds = max(5 - getLevel() / 2, 2U);

  m_player = std::make_unique<TunnelMan>(this, m_earthMap);
  createEarth();

  // devide the earth into multiple 4*4 blocks to put the boulders, golds,
  // barrels
  vector<Position> blocks;
  for (int row = 0; row < VIEW_HEIGHT - 4; row = row + 4) {
    int col = 1;
    while (col < 26) {
      blocks.push_back({col, row});
      col += 4;
    }
    col = 35;
    while (col < VIEW_WIDTH - 4) {
      blocks.push_back({col, row});
      col += 4;
    }
  }
  random_device rd;
  mt19937 g(rd());
  std::shuffle(blocks.begin(), blocks.end(), g);

  // add boulders
  for (int i = 0; i < m_numBoulders; ++i) {
    const auto& position = blocks.back();
    m_actors.emplace_back(make_unique<Boulder>(this, position.x, position.y));
    blocks.pop_back();
  }

  // add golds
  for (int i = 0; i < m_numGolds; ++i) {
    const auto& position = blocks.back();
    m_actors.emplace_back(
        make_unique<Gold>(this, position.x, position.y, false, 0));
    blocks.pop_back();
  }

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
        m_earthMap.back().emplace_back(make_unique<Earth>(this, col, row));
      } else {
        m_earthMap.back().emplace_back(nullptr);
      }
    }
  }
}

void StudentWorld::createBoulders() {
  int num = min(getLevel() / 2 + 2, 9U);
  for (int i = 0; i < num; ++i) {
    int x = rand() % (VIEW_HEIGHT - 4);
    int y = rand() % (VIEW_HEIGHT - 4);
    m_actors.emplace_back(make_unique<Boulder>(this, x, y));
  }
}

void StudentWorld::createGolds() {
  int num = min(5 - getLevel() / 2, 2U);
  for (int i = 0; i < num; ++i) {
  }
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
  int water = 0;
  int gold = 0;
  int sonar = 0;
  int oil = 3;
  ostringstream oss;
  oss << "Scr: " << setw(6) << setfill('0') << getScore()
      << "  Lvl: " << setw(2) << setfill(' ') << getLevel()
      << "  Lives: " << setw(1) << getLives() << "  Hlth: " << setw(3) << health
      << "%  Wtr: " << setw(2) << water << "  Gld: " << setw(2) << gold
      << "  Sonar: " << setw(2) << sonar << "  Oil Left: " << setw(2) << oil;
  setGameStatText(oss.str());
}
