#include "Actor.h"

//////////////////////////////////////////////////////////////////////////////
// Actor
Actor::Actor(int imageID,
             int startX,
             int startY,
             Direction dir,
             double size,
             unsigned int depth)
    : GraphObject(imageID, startX, startY, dir, size, depth) {}

Actor::~Actor() {}

//////////////////////////////////////////////////////////////////////////////
// TunnelMan

/**
 * @brief Constructor of TunnelMan
 * 1. The TunnelMan must have an image ID of TID_PLAYER.
 * 2. The TunnelMan must always start at location x=30, y=60.
 * 3. The TunnelMan, in its default state:
 *  a. Is given 10 hit points
 *  b. Is given 5 units of water to squirt with his squirt gun (he may pick up
 * additional Water in the oil field)
 *  c. Is given 1 sonar charge
 *  d. Starts out with zero gold nuggets
 *  e. Should start facing rightward
 * 4. The TunnelMan has the following graphic parameters:
 *  a. It has an image depth of 0 ¨C meaning its graphic image should always be
 * in the foreground (above other images) b. It has a size of 1.0
 */
TunnelMan::TunnelMan() : Actor(TID_PLAYER, 30, 60) {
  setVisible(true);
}

void TunnelMan::doSomething() {
  // check it is still alive
  if (!alive()) {
    return;
  }

  // check it overlaps with an earth object
  // TODO

  // check user input
  // TODO
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

bool TunnelMan::alive() const {
  return m_hitPoints > 0;
}

//////////////////////////////////////////////////////////////////////////////
// Earth
/**
 * @brief Constructor of Earch
 * A Earth object must have an image ID of TID_EARTH.
 * Each Earth object must have its x,y location specified for it
 * Each Earth object must start out facing rightward.
 * Earth has the following graphic parameters:
 *  It has an image depth of 3 ¨C meaning its graphic image should always be in
 *  the background (all other game objects have smaller depth values)
 * It has a size of .25, meaning it occupies only a 1x1 square in the oil field.
 * Earth object must set itself to be visible
 *
 * @param startX
 * @param startY
 */
Earth::Earth(int startX, int startY)
    : Actor(TID_EARTH, startX, startY, GraphObject::right, 0.25, 3) {
  setVisible(true);
}