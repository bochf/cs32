#include "CricketTeam.h"

#include <cassert>
#include <string>

using namespace std;

// equal
bool EQ(const string &fn1,
        const string &ln1,
        const string &fn2,
        const string &ln2) {
  return (fn1 == fn2 && ln1 == ln2);
}

// less than
bool LT(const string &fn1,
        const string &ln1,
        const string &fn2,
        const string &ln2) {
  return ((ln1 < ln2) || (ln1 == ln2 && fn1 < fn2));
}

// wildcard supported match
bool MATCH(const string &ori, const string &dest) {
  return (ori == dest || dest == "*");
}

bool CricketTeam::Cricketer::equalsTo(const std::string &fn,
                                      const std::string &ln) const {
  return EQ(firstName, lastName, fn, ln);
}

bool CricketTeam::Cricketer::lessThan(const std::string &fn,
                                      const std::string &ln) const {
  return LT(firstName, lastName, fn, ln);
}

CricketTeam::CricketTeam() {
  start->next = rear;
  rear->prev  = start;
}

CricketTeam::CricketTeam(const CricketTeam &other) {
  start->next = rear;
  rear->prev  = start;

  for (auto p = other.start->next; p != other.rear; p = p->next) {
    const auto &value = p->value;
    insertNode(rear, value.firstName, value.lastName, value.cricketType);
  }
}

CricketTeam::~CricketTeam() {
  Node *p = start;
  Node *next;
  while (p != nullptr) {
    next = p->next;
    delete p;
    p = next;
  }
}

CricketTeam &CricketTeam::operator=(const CricketTeam &other) {
  int   index = 0;
  Node *p     = start->next;
  Node *q     = other.start->next;

  // copy N nodes where N = min(count, other.count)
  while (p != rear && index < other.count) {
    p->value = q->value;
    p        = p->next;
    q        = q->next;
    ++index;
  }

  // add new nodes to the list if other has more
  while (index++ < other.count) {
    insertNode(p, q->value.firstName, q->value.lastName, q->value.cricketType);
    q = q->next;
  }

  // delete extra nodes
  while (p != rear) {
    q = p->next;
    removeNode(p);
    p = q;
  }

  return *this;
}

bool CricketTeam::addCricketer(const std::string &firstName,
                               const std::string &lastName,
                               const CricketType &value) {
  auto p = lowerBound(firstName, lastName);

  if (p->value.equalsTo(firstName, lastName)) {
    // the player with the exact first and last name already in the list
    return false;
  }

  insertNode(p, firstName, lastName, value);
  return true;
}

bool CricketTeam::substituteCricketer(const std::string &firstName,
                                      const std::string &lastName,
                                      const CricketType &value) {
  auto p = lowerBound(firstName, lastName);
  if (p == rear) {
    return false;
  }

  p->value.cricketType = value;
  return true;
}

bool CricketTeam::addOrSubstitute(const std::string &firstName,
                                  const std::string &lastName,
                                  const CricketType &value) {
  if (auto p = lowerBound(firstName, lastName);
      p->value.equalsTo(firstName, lastName)) {
    p->value.cricketType = value;
  } else {
    insertNode(p, firstName, lastName, value);
  }

  return true;
}

bool CricketTeam::releaseCricketer(const std::string &firstName,
                                   const std::string &lastName) {
  if (Node *p = getNode(firstName, lastName)) {
    removeNode(p);
    return true;
  }
  return false;
}

bool CricketTeam::rosteredOnCricketTeam(const std::string &firstName,
                                        const std::string &lastName) const {
  return (nullptr != getNode(firstName, lastName));
}

bool CricketTeam::searchForCricketer(const std::string &firstName,
                                     const std::string &lastName,
                                     CricketType       &value) const {
  if (Node const *p = getNode(firstName, lastName)) {
    value = p->value.cricketType;
    return true;
  }
  return false;
}

bool CricketTeam::checkTeamForCricketer(int          i,
                                        std::string &firstName,
                                        std::string &lastName,
                                        CricketType &value) const {
  if (i < 0 || i >= count) {
    return false;
  }

  Node *p = start->next;
  while (i--) {
    p = p->next;
  }
  firstName = p->value.firstName;
  lastName  = p->value.lastName;
  value     = p->value.cricketType;
  return true;
}

void CricketTeam::tradeCricketTeams(CricketTeam &other) {
  Node *p     = start;
  start       = other.start;
  other.start = p;

  p          = rear;
  rear       = other.rear;
  other.rear = p;

  int c       = count;
  count       = other.count;
  other.count = c;
}

/*************************************
 * private methods
 *************************************/
void CricketTeam::insertNode(Node              *pos,
                             const string      &firstName,
                             const string      &lastName,
                             const CricketType &value) {
  // we will never add a new node before the start
  assert(pos);
  assert(pos->prev);

  Node *newNode = new Node{
      {firstName, lastName, value}, // Cricketer
      pos->prev, // previous
      pos  // next
  };
  pos->prev->next = newNode;
  pos->prev       = newNode;
  ++count;
}

void CricketTeam::removeNode(Node *pos) {
  assert(pos);
  assert(pos->prev);
  assert(pos->next);

  pos->next->prev = pos->prev;
  pos->prev->next = pos->next;
  delete pos;
  --count;
}

/*************************************
 * non-member functions
 *************************************/
bool mergeCricketers(const CricketTeam &odOne,
                     const CricketTeam &odTwo,
                     CricketTeam       &odJoined) {
  odJoined = odOne; // copy nodes from odOne to odJoined

  bool        result = true; // the return value
  string      firstName;     // first name of the node in odTwo
  string      lastName;      // last name of the node in odTwo
  CricketType value;         // cricket type of the node in odTwo
  CricketType tmpValue;      // temp variable to same the cricket type
  for (int i = 0; i < odTwo.cricketerCount(); ++i) {
    // for each node in the odTwo, get the cricketer information
    odTwo.checkTeamForCricketer(i, firstName, lastName, value);

    // search it in odJoined
    if (odJoined.searchForCricketer(firstName, lastName, tmpValue)) {
      if (value != tmpValue) {
        // remove the conflict node
        odJoined.releaseCricketer(firstName, lastName);
        result = false;
      }
    } else {
      // add new node to the odJoined
      odJoined.addCricketer(firstName, lastName, value);
    }
  }
  return result;
}

void checkCricketers(const std::string &fsearch,
                     const std::string &lsearch,
                     const CricketTeam &odOne,
                     CricketTeam       &odResult) {
  if (fsearch == "*" && lsearch == "*") {
    odResult = odOne;
    return;
  }

  CricketTeam temp(odOne); // make a copy of odOne

  // remove all the nodes which don't match the search criteria
  string      firstName;
  string      lastName;
  CricketType value;
  for (int i = 0; i < odOne.cricketerCount(); ++i) {
    odOne.checkTeamForCricketer(i, firstName, lastName, value);
    if (!MATCH(firstName, fsearch) || !MATCH(lastName, lsearch)) {
      temp.releaseCricketer(firstName, lastName);
    }
  }

  odResult.tradeCricketTeams(temp); // swap temp and odResult
}
