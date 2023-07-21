#include "CricketTeam.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace ::testing;
using namespace std;

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

TEST(TestDoubleLink, defaultConstructor) {
  // WHEN
  CricketTeam team;

  // THEN
  EXPECT_TRUE(team.noTeam());
  EXPECT_EQ(0, team.cricketerCount());
}

TEST(TestDoubleLink, copyConstructor) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  // WHEN
  CricketTeam team2(team1);

  // THEN
  // no change in team1
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));
  // and team2 equals to team1
  EXPECT_TRUE(equal(team1, team2));
}

TEST(TestDoubleLink, assignShortToLong) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 30);
  ASSERT_EQ(30, team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  CricketTeam team2;
  fillTeam(team2, DATA, 30, 40);
  ASSERT_EQ(10, team2.cricketerCount());
  ASSERT_TRUE(sorted(team2));

  // WHEN
  team1 = team2;

  // THEN
  // no change in team2
  ASSERT_EQ(10, team2.cricketerCount());
  ASSERT_TRUE(sorted(team2));
  // and team1 equals to team2
  EXPECT_TRUE(equal(team1, team2));
}

TEST(TestDoubleLink, assignLongToShort) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 30);
  ASSERT_EQ(30, team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  CricketTeam team2;
  fillTeam(team2, DATA, 30, 40);
  ASSERT_EQ(10, team2.cricketerCount());
  ASSERT_TRUE(sorted(team2));

  // WHEN
  team2 = team1;

  // THEN
  // no change in team1
  ASSERT_EQ(30, team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));
  // and team1 equals to team2
  EXPECT_TRUE(equal(team1, team2));
}

TEST(TestDoubleLink, assignToBlankList) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  CricketTeam team2;
  ASSERT_TRUE(team2.noTeam());

  // WHEN
  team2 = team1;

  // THEN
  // no change in team1
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));
  // and team1 equals to team2
  EXPECT_TRUE(equal(team1, team2));
}

TEST(TestDoubleLink, assignBlankList) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  CricketTeam team2;
  ASSERT_TRUE(team2.noTeam());

  // WHEN
  team1 = team2;

  // THEN
  // no change in team2
  ASSERT_TRUE(team2.noTeam());
  // and team1 equals to team2
  EXPECT_TRUE(equal(team1, team2));
}

TEST(TestDoubleLink, assignSelf) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, DATA.size());
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));

  // WHEN
  team1 = team1;

  // THEN
  // no change in team1
  ASSERT_FALSE(team1.noTeam());
  ASSERT_EQ(DATA.size(), team1.cricketerCount());
  ASSERT_TRUE(sorted(team1));
}

TEST(TestDoubleLink, addDuplicate) {
  // GIVEN
  CricketTeam team;
  fillTeam(team, DATA, 0, DATA.size());
  ASSERT_EQ(DATA.size(), team.cricketerCount());
  ASSERT_TRUE(sorted(team));

  // WHEN
  bool result = team.addCricketer(DATA[0][0], DATA[0][1], ""); // duplicate

  // THEN
  EXPECT_FALSE(result);
  ASSERT_EQ(DATA.size(), team.cricketerCount()); // no change
  ASSERT_TRUE(sorted(team));

  // WHEN
  EXPECT_TRUE(team.releaseCricketer(DATA[0][0], DATA[0][1])); // remove the node
  result = team.addCricketer(DATA[0][0], DATA[0][1], "");     // no duplicate

  // THEN
  EXPECT_TRUE(result); // add success
}

TEST(TestDoubleLink, remove) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);
  ASSERT_EQ(count, team.cricketerCount());
  ASSERT_TRUE(sorted(team));

  for (const auto &name : DATA) {
    // WHEN
    bool result = team.releaseCricketer(name[0], name[1]);
    // THEN
    EXPECT_TRUE(result);                       // success
    EXPECT_EQ(--count, team.cricketerCount()); // size decreased
    EXPECT_TRUE(sorted(team));                 // keep the order

    // WHEN
    result = team.releaseCricketer(name[0], name[1]); // delete again
    // THEN
    EXPECT_FALSE(result);                    // failed
    EXPECT_EQ(count, team.cricketerCount()); // no change
  }

  EXPECT_TRUE(team.noTeam());
}

TEST(TestDoubleLink, substitut) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);
  ASSERT_EQ(count, team.cricketerCount());
  ASSERT_TRUE(sorted(team));

  const CricketType cricketType("sports lover");
  for (const auto &name : DATA) {
    // WHEN
    bool result = team.substituteCricketer(name[0], name[1], cricketType);
    // THEN
    EXPECT_TRUE(result);                     // success
    EXPECT_EQ(count, team.cricketerCount()); // size decreased
    EXPECT_TRUE(sorted(team));               // keep the order

    // WHEN
    CricketType temp;
    result =
        team.searchForCricketer(name[0], name[1], temp); // get crickter type
    // THEN
    EXPECT_TRUE(result);
    EXPECT_EQ(cricketType, temp); // cricket type changed to new type
  }
}

TEST(TestDoubleLink, roster) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);
  ASSERT_EQ(count, team.cricketerCount());
  ASSERT_TRUE(sorted(team));

  for (const auto &name : DATA) {
    // WHEN
    bool result = team.rosteredOnCricketTeam(name[0], name[1]);
    // THEN
    EXPECT_TRUE(result); // everybody is in the list
  }
}

TEST(TestDoubleLink, trade) {
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
  EXPECT_TRUE(equal(team1, copy2));
  EXPECT_TRUE(equal(team2, copy1));
}

TEST(TestDoubleLink, tradeWithBlank) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);
  CricketTeam copy1(team1); // copy for check after trade

  CricketTeam team2; // empty team

  // WHEN
  team1.tradeCricketTeams(team2);

  // THEN
  EXPECT_TRUE(team1.noTeam());
  EXPECT_TRUE(equal(team2, copy1));
}

TEST(TestDoubleLink, tradeSelf) {
  // GIVEN
  CricketTeam team;
  fillTeam(team, DATA, 0, 10);
  CricketTeam copy(team); // copy for check after trade

  // WHEN
  team.tradeCricketTeams(team);

  // THEN
  EXPECT_TRUE(equal(team, copy));
}

TEST(TestDoubleLink, mergeNoOverlap) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 20, 35);

  // WHEN
  CricketTeam joined;
  bool        result = mergeCricketers(team1, team2, joined);

  // THEN
  EXPECT_TRUE(result);
  EXPECT_EQ(joined.cricketerCount(),
            team1.cricketerCount() + team2.cricketerCount());
  EXPECT_TRUE(sorted(joined));
  EXPECT_EQ(10, team1.cricketerCount());
  EXPECT_EQ(15, team2.cricketerCount());
}

TEST(TestDoubleLink, mergeHasOverlap) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);

  // WHEN
  CricketTeam joined;
  bool        result = mergeCricketers(team1, team2, joined);

  // THEN
  EXPECT_TRUE(result);
  EXPECT_EQ(joined.cricketerCount(), 35);
  EXPECT_TRUE(sorted(joined));
  EXPECT_EQ(10, team1.cricketerCount());
  EXPECT_EQ(30, team2.cricketerCount());
}

TEST(TestDoubleLink, mergeHasConflict) {
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
  EXPECT_FALSE(result);
  EXPECT_EQ(joined.cricketerCount(), 34);
  EXPECT_TRUE(sorted(joined));
  EXPECT_EQ(10, team1.cricketerCount());
  EXPECT_EQ(30, team2.cricketerCount());
}

TEST(TestDoubleLink, mergeToNonEmptyList) {
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
  EXPECT_TRUE(result);
  EXPECT_EQ(joined.cricketerCount(), 35);
  EXPECT_TRUE(sorted(joined));
  EXPECT_EQ(10, team1.cricketerCount());
  EXPECT_EQ(30, team2.cricketerCount());
}

TEST(TestDoubleLink, mergeToSelf) {
  // GIVEN
  CricketTeam team1;
  fillTeam(team1, DATA, 0, 10);

  CricketTeam team2;
  fillTeam(team2, DATA, 5, 35);

  // WHEN
  bool result = mergeCricketers(team1, team2, team1);

  // THEN
  EXPECT_TRUE(result);
  EXPECT_EQ(team1.cricketerCount(), 35);
  EXPECT_TRUE(sorted(team1));
  EXPECT_EQ(30, team2.cricketerCount());
}

TEST(TestDoubleLink, search) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  CricketTeam result;
  for (const auto &cricketer : DATA) {
    // WHEN
    checkCricketers(cricketer[0], cricketer[1], team, result);

    // THEN
    EXPECT_EQ(1, result.cricketerCount());
    string      firstName;
    string      lastName;
    CricketType cricketType;
    EXPECT_TRUE(
        result.checkTeamForCricketer(0, firstName, lastName, cricketType));
    EXPECT_EQ(firstName, cricketer[0]);
    EXPECT_EQ(lastName, cricketer[1]);
    EXPECT_EQ(cricketType, cricketer[2]);
  }
}

TEST(TestDoubleLink, searchWildcard) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);
  CricketTeam result;

  // WHEN
  checkCricketers("*", "Johnson", team, result);

  // THEN
  EXPECT_EQ(2, result.cricketerCount());
  EXPECT_TRUE(result.rosteredOnCricketTeam("Andrew", "Johnson"));
  EXPECT_TRUE(result.rosteredOnCricketTeam("Lyndon", "Johnson"));

  // WHEN
  checkCricketers("*", "*", team, result);
  EXPECT_TRUE(equal(result, team));
}

TEST(TestDoubleLink, searchAndSaveResultInPlace) {
  // GIVEN
  CricketTeam team;
  size_t      count = DATA.size();
  fillTeam(team, DATA, 0, count);

  // WHEN
  checkCricketers("*", "Johnson", team, team);

  // THEN
  EXPECT_EQ(2, team.cricketerCount());
  EXPECT_TRUE(team.rosteredOnCricketTeam("Andrew", "Johnson"));
  EXPECT_TRUE(team.rosteredOnCricketTeam("Lyndon", "Johnson"));
  EXPECT_FALSE(team.rosteredOnCricketTeam("George", "Washington"));
}