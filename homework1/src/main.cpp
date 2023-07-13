#include "LinkedList.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define ASSERT_EQ(x, y)    assert((x) == (y))
#define ASSERT_TRUE(cond)  assert(cond)
#define ASSERT_FALSE(cond) assert(!(cond))

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

void testDefaultConstructor() {
  LinkedList ll;

  assert(0 == ll.size());

  string item;
  ASSERT_FALSE(ll.get(0, item));
}

void testCopyConstructor() {
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
  ASSERT_TRUE(equal(first, second));
}

void testAssignment() {
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
  ASSERT_TRUE(equal(first, second));
}

void testAssignBlankList() {
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
  ASSERT_TRUE(equal(first, second));
}

void testAssignToBlankList() {
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
  ASSERT_TRUE(equal(first, second));
}

void testAppend() {
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
  ASSERT_EQ(5, first.size());
  ASSERT_TRUE(first.get(3, s));
  ASSERT_EQ("Batman", s);

  ASSERT_EQ(2, second.size());
  ASSERT_TRUE(second.get(1, s));
  ASSERT_EQ("Robin", s);
}

void testAppendBlankList() {
  // GIVEN
  LinkedList first;
  LinkedList empty;
  first.insertToFront("Athos");
  first.insertToFront("Porthos");
  first.insertToFront("Aramis");

  // WHEN
  first.append(empty); // adds contents of empty to the end of first

  // THEN
  ASSERT_EQ(3, first.size());
  ASSERT_EQ(0, empty.size());
}

void testAppendToBlankList() {
  // GIVEN
  LinkedList empty;
  LinkedList second;
  second.insertToFront("Robin");
  second.insertToFront("Batman");

  // WHEN
  empty.append(second); // adds contents of second to the end of empty

  // THEN
  ASSERT_TRUE(equal(empty, second));
}

void testGet() {
  // GIVEN
  LinkedList ll;
  string     item;

  // THEN
  ASSERT_FALSE(ll.get(0, item));

  // GIVEN
  ll.insertToFront("abc");
  ASSERT_FALSE(ll.get(1, item));
  ASSERT_TRUE(ll.get(0, item));
  ASSERT_EQ("abc", item);
}

void testSwap() {
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
  ASSERT_TRUE(equal(first, secondCopy));
  ASSERT_TRUE(equal(second, firstCopy));
}

void testSize() {
  // GIVEN
  LinkedList ll;

  // THEN
  ASSERT_EQ(0, ll.size());

  // WHEN
  ll.insertToFront("Hawkeye");
  ll.insertToFront("Thor");
  ll.insertToFront("Hulk");
  ll.insertToFront("Black Widow");
  ll.insertToFront("Iron Man");
  ll.insertToFront("Captain America");

  // THEN
  ASSERT_EQ(6, ll.size());
}

class OutputCapture {
public:
  // constructor and destroctor
  OutputCapture() {
    // Save cout's buffer here
    m_oriBuffer = cout.rdbuf();

    // Redirect cout to our stringstream buffer or any other ostream
    cout.rdbuf(m_ssCout.rdbuf());
  };

  virtual ~OutputCapture() { cout.rdbuf(m_oriBuffer); };

  // cleanup the buffer
  void clear() { m_ssCout.str(""); };
  // get the string stored in the buffer
  std::string str() const { return m_ssCout.str(); };

private:
  // disable copy and move constructor
  OutputCapture(const OutputCapture &rhs)            = delete;
  OutputCapture(const OutputCapture &&rhs)           = delete;
  OutputCapture &operator=(const OutputCapture &rhs) = delete;

  stringstream m_ssCout;
  streambuf   *m_oriBuffer;
};

void testPrint() {
  OutputCapture capture;

  // GIVEN
  LinkedList list;
  list.insertToFront("Hawkeye");
  list.insertToFront("Thor");
  list.insertToFront("Hulk");
  list.insertToFront("Black Widow");
  list.insertToFront("Iron Man");
  list.insertToFront("Captain America");

  // WHEN
  list.printList();

  // THEN
  ASSERT_EQ("Captain America Iron Man Black Widow Hulk Thor Hawkeye\n",
            capture.str());
}

void testPrintReverse() {
  OutputCapture capture;

  // GIVEN
  LinkedList list;
  list.insertToFront("Hawkeye");
  list.insertToFront("Thor");
  list.insertToFront("Hulk");
  list.insertToFront("Black Widow");
  list.insertToFront("Iron Man");
  list.insertToFront("Captain America");
  LinkedList reverse = list;
  reverse.reverseList();

  // WHEN
  list.printReverse();
  string output = capture.str();

  capture.clear();
  reverse.printList();

  // THEN
  ASSERT_EQ(output, capture.str());
}

void testPrintEmptyList() {
  OutputCapture capture;

  // GIVEN
  LinkedList empty;

  // WHEN
  empty.printList();

  // THEN
  ASSERT_EQ("\n", capture.str());
}

int main() {
  testDefaultConstructor();

  return 0;
}