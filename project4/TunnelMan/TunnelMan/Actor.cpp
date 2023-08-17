#include "Actor.h"
#include "GameConstants.h"
#include "GameController.h"
#include "StudentWorld.h"

#include <memory>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Actor
Actor::Actor(StudentWorld* world,
             int imageID,
             int x,
             int y,
             bool visible,
             Direction dir,
             unsigned int depth,
             double size)
    : GraphObject(imageID, x, y, dir, size, depth), m_world(world) {
  setVisible(visible);
}

Actor::~Actor() {
  m_world = nullptr;
}

void Actor::getArea(vector<Position>& area) const {
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

//////////////////////////////////////////////////////////////////////////////
// TunnelMan
void TunnelMan::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  // if overlap any earth object, dig it
  if (dig()) {
    Game().playSound(SOUND_DIG);
    return;
  }

  // check user input
  int key;
  if (Game().getLastKey(key)) {
    switch (key) {
      case KEY_PRESS_ESCAPE:  // abort current level
        m_hitPoints = 0;      // dead
        break;
      case KEY_PRESS_SPACE:  // fire a squirt
        if (m_waterUnits <= 0) {
          return;
        }
        switch (getDirection()) {
          case Direction::left:
            m_world->addActor(make_unique<Squirt>(m_world, getX() - 4, getY(),
                                                  getDirection()));
            break;
          case right:
            m_world->addActor(make_unique<Squirt>(m_world, getX() + 4, getY(),
                                                  getDirection()));
            break;
          case up:
            m_world->addActor(make_unique<Squirt>(m_world, getX(), getY() + 4,
                                                  getDirection()));
            break;
          case down:
            m_world->addActor(make_unique<Squirt>(m_world, getX(), getY() - 4,
                                                  getDirection()));
            break;
          default:
            break;
        }
        Game().playSound(SOUND_PLAYER_SQUIRT);
        break;
      case KEY_PRESS_LEFT:
        break;
      case KEY_PRESS_RIGHT:
        break;
      case KEY_PRESS_UP:
        break;
      case KEY_PRESS_DOWN:
        break;
      case 'Z':
      case 'z':
        break;
      case KEY_PRESS_TAB:
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

bool TunnelMan::dig() const {
  bool hasEarth = false;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (m_earthMap[getY() + i][getX() + j]) {
        hasEarth = true;
        m_earthMap[getY() + i][getX() + j] = nullptr;
      }
    }
  }

  return hasEarth;
}

//////////////////////////////////////////////////////////////////////////////
// Earth

//////////////////////////////////////////////////////////////////////////////
// Boulder
void Boulder::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  switch (m_state) {
    case State::stable: {
    } break;
    case State::falling: {
    } break;
    case State::waiting: {
      if (m_waitTicks < 30) {
        ++m_waitTicks;
      } else {
        Game().playSound(SOUND_FALLING_ROCK);
        m_state = State::falling;
      }
    } break;
    case State::dead: {
    } break;
    default:
      break;
  }
}