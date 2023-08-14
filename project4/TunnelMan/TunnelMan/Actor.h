#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject {
 public:
  Actor(int imageID,
        int startX,
        int startY,
        Direction dir = right,
        double size = 1.0,
        unsigned int depth = 0);
  virtual ~Actor();

  virtual void doSomething() = 0;
};

class TunnelMan : public Actor {
 public:
  TunnelMan();

  void doSomething() final;
};

class Earth : public Actor {
 public:
  Earth(int startX, int startY);

  void doSomething() final{};
};

class Boulder : public Actor {
 public:
  void doSomething() final{};
};

class Squirt : public Actor {
 public:
  void doSomething() final{};
};

class Barrel : public Actor {
 public:
  void doSomething() final{};
};

class Gold : public Actor {
 public:
  void doSomething() final{};
};

class Sonar : public Actor {
 public:
  void doSomething() final{};
};

class WaterPool : public Actor {
 public:
  void doSomething() final{};
};

class Protester : public Actor {};

class RegularProtester : public Protester {
 public:
  void doSomething() final{};
};

class HardcoreProtester : public Protester {
 public:
  void doSomething() final{};
};

#endif  // ACTOR_H_
