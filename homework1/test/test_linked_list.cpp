///
/// test LinkedList
///

#include "LinkedList.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace std;

bool equal(const LinkedList &first, const LinkedList &second) {
  if (first.size() != second.size())
    return false;

  for (int i = 0; i < first.size(); ++i) {
    string item1;
    string item2;
    if (!first.get(i, item1))
      return false;
    if (!second.get(i, item2))
      return false;
    if (item1 != item2)
      return false;
  }
  return true;
}

TEST(TestLinkedList, DefaultConstructor) {
  LinkedList ll;

  ASSERT_EQ(0, ll.size());

  string item;
  EXPECT_FALSE(ll.get(0, item));
}

TEST(TestLinkedList, CopyConstructor) {
  // GIVEN
  LinkedList first;
  first.insertToFront("Hawkeye");
  first.insertToFront("Thor");
  first.insertToFront("Hulk");
  first.insertToFront("Black Widow");
  first.insertToFront("Iron Man");
  first.insertToFront("Captain America");

  // WHEN
  LinkedList second(first);

  // THEN
  EXPECT_TRUE(equal(first, second));
}

TEST(TestLinkedList, Assignment) {
  // GIVEN
  LinkedList first;
  first.insertToFront("Hawkeye");
  first.insertToFront("Thor");
  first.insertToFront("Hulk");
  first.insertToFront("Black Widow");
  first.insertToFront("Iron Man");
  first.insertToFront("Captain America");

  LinkedList second;
  second.insertToFront("The Mandalorian");
  second.insertToFront("Baby Yoda");
  second.insertToFront("Cara Dune");
  second.insertToFront("Greef Karga");

  // WHEN
  second = first;

  // THEN
  EXPECT_TRUE(equal(first, second));
}

TEST(TestLinkedList, AssignBlankList) {
  // GIVEN
  LinkedList first;
  LinkedList second;
  second.insertToFront("The Mandalorian");
  second.insertToFront("Baby Yoda");
  second.insertToFront("Cara Dune");
  second.insertToFront("Greef Karga");

  // WHEN
  second = first;

  // THEN
  EXPECT_TRUE(equal(first, second));
}

TEST(TestLinkedList, AssignToBlankList) {
  // GIVEN
  LinkedList first;
  first.insertToFront("Hawkeye");
  first.insertToFront("Thor");
  first.insertToFront("Hulk");
  first.insertToFront("Black Widow");
  first.insertToFront("Iron Man");
  first.insertToFront("Captain America");

  // WHEN
  LinkedList second = first;

  // THEN
  EXPECT_TRUE(equal(first, second));
}

TEST(TestLinkedList, Append) {
  // GIVEN
  LinkedList first;
  first.insertToFront("Athos");
  first.insertToFront("Porthos");
  first.insertToFront("Aramis");
  LinkedList second;
  second.insertToFront("Robin");
  second.insertToFront("Batman");

  // WHEN
  first.append(second); // adds contents of second to the end of first

  // THEN
  string s;
  EXPECT_EQ(5, first.size());
  EXPECT_TRUE(first.get(3, s));
  EXPECT_EQ("Batman", s);

  EXPECT_EQ(2, second.size());
  EXPECT_TRUE(second.get(1, s));
  EXPECT_EQ("Robin", s);
}

TEST(TestLinkedList, AppendBlankList) {
  // GIVEN
  LinkedList first;
  LinkedList empty;
  first.insertToFront("Athos");
  first.insertToFront("Porthos");
  first.insertToFront("Aramis");

  // WHEN
  first.append(empty); // adds contents of empty to the end of first

  // THEN
  EXPECT_EQ(3, first.size());
  EXPECT_EQ(0, empty.size());
}

TEST(TestLinkedList, AppendToBlankList) {
  // GIVEN
  LinkedList empty;
  LinkedList second;
  second.insertToFront("Robin");
  second.insertToFront("Batman");

  // WHEN
  empty.append(second); // adds contents of second to the end of empty

  // THEN
  EXPECT_TRUE(equal(empty, second));
}

TEST(TestLinkedList, Get) {
  // GIVEN
  LinkedList ll;
  string     item;

  // THEN
  EXPECT_FALSE(ll.get(0, item));

  // GIVEN
  ll.insertToFront("abc");
  EXPECT_FALSE(ll.get(1, item));
  EXPECT_TRUE(ll.get(0, item));
  EXPECT_EQ("abc", item);
}

TEST(TestLinkedList, Swap) {
  // GIVEN
  LinkedList first;
  first.insertToFront("A");
  first.insertToFront("B");
  first.insertToFront("C");
  first.insertToFront("D");
  const auto firstCopy = first;
  LinkedList second;
  second.insertToFront("X");
  second.insertToFront("Y");
  second.insertToFront("Z");
  const auto secondCopy = second;

  // WHEN
  first.swap(second); // exchange contents of first and second

  // THEN
  EXPECT_TRUE(equal(first, secondCopy));
  EXPECT_TRUE(equal(second, firstCopy));
}

TEST(TestLinkedList, Size) {
  // GIVEN
  LinkedList ll;

  // THEN
  EXPECT_EQ(0, ll.size());

  // WHEN
  ll.insertToFront("Hawkeye");
  ll.insertToFront("Thor");
  ll.insertToFront("Hulk");
  ll.insertToFront("Black Widow");
  ll.insertToFront("Iron Man");
  ll.insertToFront("Captain America");

  // THEN
  EXPECT_EQ(6, ll.size());
}
