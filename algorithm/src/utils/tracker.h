#ifndef __TRACKER__
#define __TRACKER__

#include <string>
#include <chrono>

namespace CS32
{
  class Tracker
  {
  public:
    virtual ~Tracker(){};

    virtual void increaseStep() = 0;
  };

  class NoopTracker : public Tracker
  {
  public:
    virtual void increaseStep() override{};
  };

  class TimeTracker : public Tracker
  {
  public:
    TimeTracker(const std::string &algorithmName, int dataSize);
    virtual ~TimeTracker();

    void increaseStep() override
    {
      ++m_steps;
    }

  private:
    std::string m_algorithmName;
    int m_dataSize;
    int m_steps;
    std::chrono::time_point<std::chrono::steady_clock> m_start;

    void stop();
  };
}
#endif