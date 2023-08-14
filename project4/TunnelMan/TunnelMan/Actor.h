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

  virtual void doSomething() = 0;  // to be implemented in the sub classes
  virtual void annoyed(int deduction){};  // do nothing by default
};

class TunnelMan : public Actor {
 public:
  TunnelMan();

  void doSomething() final;
  void annoyed(int deduction) final;

 private:
  int m_hitPoints = 10;
  int m_waterUnits = 5;
  int m_sonarCharges = 1;
  int m_goldNuggets = 0;

  bool alive() const;
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
