#include "CricketTeam.h"

CricketTeam::CricketTeam() { root.m_prev = &root; }

bool CricketTeam::noTeam() const { return root.m_next == nullptr; }
int  CricketTeam::cricketerCount() const { return m_count; }

bool CricketTeam::addCricketer(const std::string &firstName,
                               const std::string &lastName,
                               const CricketType &value) {
  Cricketer cricketer = {firstName, lastName, value};

  Node *p = &root;
  while (p != nullptr && p->m_value < cricketer)
    ;

  return false;
}

bool CricketTeam::substituteCricketer(const std::string &firstName,
                                      const std::string &lastName,
                                      const CricketType &value) {
  return false;
}

bool CricketTeam::addOrSubstitute(const std::string &firstName,
                                  const std::string &lastName,
                                  const CricketType &value) {
  return false;
}

bool CricketTeam::releaseCricketer(const std::string &firstName,
                                   const std::string &lastName) {
  return false;
}

bool CricketTeam::rosteredOnCricketTeam(const std::string &firstName,
                                        const std::string &lastName) const {
  return false;
}

bool CricketTeam::searchForCricketer(const std::string &firstName,
                                     const std::string &lastName,
                                     CricketType       &value) const {
  return false;
}

bool CricketTeam::checkTeamForCricketer(int          i,
                                        std::string &firstName,
                                        std::string &lastName,
                                        CricketType &value) const {
  return false;
}

void CricketTeam::tradeCricketTeams(CricketTeam &other) {}
