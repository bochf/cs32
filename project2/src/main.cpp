#include "CricketTeam.h"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

#define CHECKTYPE(f, t)                                                        \
  {                                                                            \
    auto p = (t)(f);                                                           \
    (void)p;                                                                   \
  }
static_assert(std::is_default_constructible_v<CricketTeam>,
              "Map must be default-constructible.");
static_assert(std::is_copy_constructible_v<CricketTeam>,
              "Map must be copy-constructible.");

void ThisFunctionWillNeverBeCalled() {
  CHECKTYPE(&CricketTeam::operator=,
            CricketTeam & (CricketTeam::*)(const CricketTeam &));
  CHECKTYPE(&CricketTeam::noTeam, bool(CricketTeam::*)() const);
  CHECKTYPE(&CricketTeam::cricketerCount, int(CricketTeam::*)() const);
  CHECKTYPE(&CricketTeam::addCricketer,
            bool(CricketTeam::*)(
                const std::string &, const std::string &, const CricketType &));
  CHECKTYPE(&CricketTeam::substituteCricketer,
            bool(CricketTeam::*)(
                const std::string &, const std::string &, const CricketType &));
  CHECKTYPE(&CricketTeam::addOrSubstitute,
            bool(CricketTeam::*)(
                const std::string &, const std::string &, const CricketType &));
  CHECKTYPE(&CricketTeam::releaseCricketer,
            bool(CricketTeam::*)(const std::string &, const std::string &));
  CHECKTYPE(&CricketTeam::rosteredOnCricketTeam,
            bool(CricketTeam::*)(const std::string &, const std::string &)
                const);
  CHECKTYPE(&CricketTeam::searchForCricketer,
            bool(CricketTeam::*)(
                const std::string &, const std::string &, CricketType &) const);
  CHECKTYPE(&CricketTeam::checkTeamForCricketer,
            bool(CricketTeam::*)(
                int, std::string &, std::string &, CricketType &) const);
  CHECKTYPE(&CricketTeam::tradeCricketTeams,
            void(CricketTeam::*)(CricketTeam &));
  CHECKTYPE(mergeCricketers,
            bool (*)(const CricketTeam &, const CricketTeam &, CricketTeam &));
  CHECKTYPE(checkCricketers,
            void (*)(const std::string &,
                     const std::string &,
                     const CricketTeam &,
                     CricketTeam &));
}

/// @brief compare 2 cricket team
/// @param first
/// @param second
/// @return true if they are same
bool equal(const CricketTeam &first, const CricketTeam &second) {
  if (first.cricketerCount() != second.cricketerCount())
    return false;

  string fn1;
  string ln1;
  string value1;
  string fn2;
  string ln2;
  string value2;
  for (int i = 0; i < first.cricketerCount(); ++i) {
    first.checkTeamForCricketer(i, fn1, ln1, value1);
    second.checkTeamForCricketer(i, fn2, ln2, value2);
    if (fn1 != fn2 || ln1 != ln2 || value1 != value2) {
      cerr << "Cricketer " << i << " does not matche:" << endl;
      cerr << " [1]: " << fn1 << ", " << ln1 << ", " << value1 << endl;
      cerr << " [2]: " << fn2 << ", " << ln2 << ", " << value2 << endl;
      return false;
    }
  }

  return true;
}

/// @brief check all the players are sorted by last name and first name
/// @param team
/// @return ture if in the right order
bool sorted(const CricketTeam &team) {
  int count = team.cricketerCount();
  if (count < 2)
    return true;

  string      largeFN;
  string      largeLN;
  string      smallFN;
  string      smallLN;
  CricketType value;
  team.checkTeamForCricketer(--count, largeFN, largeLN, value);

  while (team.checkTeamForCricketer(--count, smallFN, smallLN, value)) {
    if ((largeLN < smallLN) || (largeLN == smallLN && largeFN < smallFN))
      return false;

    largeFN = smallFN;
    largeLN = smallLN;
  }

  return true;
}

using Cricketers      = vector<vector<string>>;
const Cricketers DATA = {
    {"George",     "Washington", "Federalist"           },
    {"John",       "Adams",      "Federalist"           },
    {"Thomas",     "Jefferson",  "Democratic-Republican"},
    {"James",      "Madison",    "Democratic-Republican"},
    {"James",      "Monroe",     "Democratic-Republican"},
    {"Andrew",     "Jackson",    "Democratic"           },
    {"Martin",     "Buren",      "Democratic"           },
    {"William",    "Harrison",   "Whig"                 },
    {"John",       "Tyler",      "Whig"                 },
    {"James",      "Polk",       "Democratic"           },
    {"Zachary",    "Taylor",     "Whig"                 },
    {"Millard",    "Fillmore",   "Whig"                 },
    {"Franklin",   "Pierce",     "Democratic"           },
    {"James",      "Buchanan",   "Democratic"           },
    {"Abraham",    "Lincoln",    "Republican"           },
    {"Andrew",     "Johnson",    "Democratic"           },
    {"Ulysses",    "Grant",      "Republican"           },
    {"Rutherford", "Hayes",      "Republican"           },
    {"James",      "Garfield",   "Republican"           },
    {"Chester",    "Arthur",     "Republican"           },
    {"Benjamin",   "Harrison",   "Republican"           },
    {"Grover",     "Cleveland",  "Democratic"           },
    {"William",    "McKinley",   "Republican"           },
    {"Theodore",   "Roosevelt",  "Republican"           },
    {"William",    "Taft",       "Republican"           },
    {"Woodrow",    "Wilson",     "Democratic"           },
    {"Warren",     "Harding",    "Republican"           },
    {"Calvin",     "Coolidge",   "Republican"           },
    {"Herbert",    "Hoover",     "Republican"           },
    {"Franklin",   "Roosevelt",  "Democratic"           },
    {"Harry",      "Truman",     "Democratic"           },
    {"Dwight",     "Eisenhower", "Republican"           },
    {"John",       "Kennedy",    "Democratic"           },
    {"Lyndon",     "Johnson",    "Democratic"           },
    {"Richard",    "Nixon",      "Republican"           },
    {"Gerald",     "Ford",       "Republican"           },
    {"Jimmy",      "Carter",     "Democratic"           },
    {"Ronald",     "Reagan",     "Republican"           },
    {"George",     "Bush",       "Republican"           },
    {"Bill",       "Clinton",    "Democratic"           },
    {"Barack",     "Obama",      "Democratic"           },
    {"Donald",     "Trump",      "Republican"           },
    {"Joe",        "Biden",      "Democratic"           }
};

/// @brief add cricketers into the team
/// @param team
/// @param data the list of tuple (firstName, lastName, type)
/// @param start
/// @param end
void fillTeam(CricketTeam      &team,
              const Cricketers &data,
              size_t            start,
              size_t            end) {
  assert(end <= data.size());

  for (size_t i = start; i < end; ++i) {
    team.addCricketer(data[i][0], data[i][1], data[i][2]);
  }
}

void test_defaultConstructor() {
  // WHEN
  CricketTeam team;

  // THEN
  assert(team.noTeam());
  assert(0 == team.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_copyConstructor() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());

  // WHEN
  CricketTeam team2(team1);

  // THEN
  // no change in team1
  assert(DATA.size() == team1.cricketerCount());
  assert(sorted(team1));
  // and team2 equals to team1
  assert(equal(team1, team2));
  cerr << __func__ << " Passed." << endl;
}

void test_assignShortToLong() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 30);

  CricketTeam team2;
  fillTeam(team2, DATA, 30, 40);

  // WHEN
  team1 = team2;

  // THEN
  // no change in team2
  assert(10 == team2.cricketerCount());
  assert(sorted(team2));
  assert(equal(team1, team2)); // and team1 equals to team2
  cerr << __func__ << " Passed." << endl;
}

void test_assignLongToShort() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 30);

  CricketTeam team2;
  fillTeam(team2, DATA, 30, 40);

  // WHEN
  team2 = team1;

  // THEN
  // no change in team1
  assert(30 == team1.cricketerCount());
  assert(sorted(team1));
  // and team1 equals to team2
  assert(equal(team1, team2));
  cerr << __func__ << " Passed." << endl;
}

void test_assignToBlankList() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());

  CricketTeam team2;
  assert(team2.noTeam());

  // WHEN
  team2 = team1;

  // THEN
  // no change in team1
  assert(DATA.size() == team1.cricketerCount());
  assert(sorted(team1));
  // and team1 equals to team2
  assert(equal(team1, team2));
  cerr << __func__ << " Passed." << endl;
}

void test_assignBlankList() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());

  CricketTeam team2;
  assert(team2.noTeam());

  // WHEN
  team1 = team2;

  // THEN
  // no change in team2
  assert(team2.noTeam());
  // and team1 equals to team2
  assert(equal(team1, team2));
  cerr << __func__ << " Passed." << endl;
}

void test_assignSelf() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());

  // WHEN
  team1 = team1;

  // THEN
  // no change in team1
  assert(!team1.noTeam());
  assert(DATA.size() == team1.cricketerCount());
  assert(sorted(team1));
  cerr << __func__ << " Passed." << endl;
}

void test_addDuplicate() {
  // GIVEN
  CricketTeam team;
  fillTeam(team, DATA, 0, DATA.size());

  // WHEN
  bool result = team.addCricketer(DATA[0][0], DATA[0][1], ""); // duplicate

  // THEN
  assert(!result);
  assert(DATA.size() == team.cricketerCount()); // no change
  assert(sorted(team));

  // WHEN
  assert(team.releaseCricketer(DATA[0][0], DATA[0][1]));  // remove the node
  result = team.addCricketer(DATA[0][0], DATA[0][1], ""); // no duplicate

  // THEN
  assert(result); // add success
  cerr << __func__ << " Passed." << endl;
}

void test_remove() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  for (const auto &name : DATA) {
    // WHEN
    bool result = team.releaseCricketer(name[0], name[1]);
    // THEN
    assert(result); // success
    --count;
    assert(count == team.cricketerCount()); // size decreased
    assert(sorted(team));                   // keep the order

    // WHEN
    result = team.releaseCricketer(name[0], name[1]); // delete again
    // THEN
    assert(!result);                        // failed
    assert(count == team.cricketerCount()); // no change
  }

  assert(team.noTeam());
  cerr << __func__ << " Passed." << endl;
}

void test_substitut() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  const CricketType cricketType("sports lover");
  for (const auto &name : DATA) {
    // WHEN
    bool result = team.substituteCricketer(name[0], name[1], cricketType);
    // THEN
    assert(result);       // success
    assert(sorted(team)); // keep the order

    // WHEN
    CricketType temp;
    result =
        team.searchForCricketer(name[0], name[1], temp); // get crickter type
    // THEN
    assert(result);
    assert(cricketType == temp); // cricket type changed to new type
  }
  cerr << __func__ << " Passed." << endl;
}

void test_roster() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  for (const auto &name : DATA) {
    // WHEN
    bool result = team.rosteredOnCricketTeam(name[0], name[1]);
    // THEN
    assert(result); // everybody is in the list
  }
  cerr << __func__ << " Passed." << endl;
}

void test_trade() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);
  CricketTeam copy1(team1); // copy for check after trade

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 20);
  CricketTeam copy2(team2); // copy for check after trade

  // WHEN
  team1.tradeCricketTeams(team2);

  // THEN
  assert(equal(team1, copy2));
  assert(equal(team2, copy1));
  cerr << __func__ << " Passed." << endl;
}

void test_tradeWithBlank() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);
  CricketTeam copy1(team1); // copy for check after trade

  CricketTeam team2; // empty team

  // WHEN
  team1.tradeCricketTeams(team2);

  // THEN
  assert(team1.noTeam());
  assert(equal(team2, copy1));
  cerr << __func__ << " Passed." << endl;
}

void test_tradeSelf() {
  // GIVEN
  CricketTeam team;
  fillTeam(team, DATA, 0, 10);
  CricketTeam copy(team); // copy for check after trade

  // WHEN
  team.tradeCricketTeams(team);

  // THEN
  assert(equal(team, copy));
  cerr << __func__ << " Passed." << endl;
}

void test_mergeNoOverlap() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 20, 35);

  // WHEN
  CricketTeam joined;
  bool        result = mergeCricketers(team1, team2, joined);

  // THEN
  assert(result);
  assert(joined.cricketerCount() ==
         team1.cricketerCount() + team2.cricketerCount());
  assert(sorted(joined));
  assert(10 == team1.cricketerCount());
  assert(15 == team2.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_mergeHasOverlap() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);

  // WHEN
  CricketTeam joined;
  bool        result = mergeCricketers(team1, team2, joined);

  // THEN
  assert(result);
  assert(joined.cricketerCount() == 35);
  assert(sorted(joined));
  assert(10 == team1.cricketerCount());
  assert(30 == team2.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_mergeHasConflict() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);
  const auto &tuple = DATA[5];
  team2.substituteCricketer(tuple[0], tuple[1], "elephant");

  // WHEN
  CricketTeam joined;
  bool        result = mergeCricketers(team1, team2, joined);

  // THEN
  assert(!result);
  assert(joined.cricketerCount() == 34);
  assert(sorted(joined));
  assert(10 == team1.cricketerCount());
  assert(30 == team2.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_mergeToNonEmptyList() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);

  // WHEN
  CricketTeam joined;
  fillTeam(joined, DATA, 0, 40);
  bool result = mergeCricketers(team1, team2, joined);

  // THEN
  assert(result);
  assert(joined.cricketerCount() == 35);
  assert(sorted(joined));
  assert(10 == team1.cricketerCount());
  assert(30 == team2.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_mergeToSelf() {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);

  // WHEN
  bool result = mergeCricketers(team1, team2, team1);

  // THEN
  assert(result);
  assert(team1.cricketerCount() == 35);
  assert(sorted(team1));
  assert(30 == team2.cricketerCount());
  cerr << __func__ << " Passed." << endl;
}

void test_search() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  CricketTeam result;
  for (const auto &cricketer : DATA) {
    // WHEN
    checkCricketers(cricketer[0], cricketer[1], team, result);

    // THEN
    assert(1 == result.cricketerCount());
    string      firstName;
    string      lastName;
    CricketType cricketType;
    assert(result.checkTeamForCricketer(0, firstName, lastName, cricketType));
    assert(firstName == cricketer[0]);
    assert(lastName == cricketer[1]);
    assert(cricketType == cricketer[2]);
  }
  cerr << __func__ << " Passed." << endl;
}

void test_searchWildcard() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);
  CricketTeam result;

  // WHEN
  checkCricketers("*", "Johnson", team, result);

  // THEN
  assert(2 == result.cricketerCount());
  assert(result.rosteredOnCricketTeam("Andrew", "Johnson"));
  assert(result.rosteredOnCricketTeam("Lyndon", "Johnson"));

  // WHEN
  checkCricketers("*", "*", team, result);
  assert(equal(result, team));
  cerr << __func__ << " Passed." << endl;
}

void test_searchAndSaveResultInPlace() {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  // WHEN
  checkCricketers("*", "Johnson", team, team);

  // THEN
  assert(2 == team.cricketerCount());
  assert(team.rosteredOnCricketTeam("Andrew", "Johnson"));
  assert(team.rosteredOnCricketTeam("Lyndon", "Johnson"));
  assert(!team.rosteredOnCricketTeam("George", "Washington"));
  cerr << __func__ << " Passed." << endl;
}

int main() {
  test_defaultConstructor();
  test_copyConstructor();
  test_assignShortToLong();
  test_assignLongToShort();
  test_assignToBlankList();
  test_assignBlankList();
  test_assignSelf();
  test_addDuplicate();
  test_remove();
  test_substitut();
  test_roster();
  test_trade();
  test_tradeWithBlank();
  test_tradeSelf();
  test_mergeNoOverlap();
  test_mergeHasOverlap();
  test_mergeHasConflict();
  test_mergeToNonEmptyList();
  test_mergeToSelf();
  test_search();
  test_searchWildcard();
  test_searchAndSaveResultInPlace();
  return 0;
}