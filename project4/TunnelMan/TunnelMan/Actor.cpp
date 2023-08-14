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
TunnelMan::TunnelMan() : Actor(TID_PLAYER, 30, 60) {}

void TunnelMan::doSomething() {}

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