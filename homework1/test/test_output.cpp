///
/// test LinkedList print to console
///

#include "LinkedList.h"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

using namespace ::testing;
using namespace std;

class CaptureCout : public Test {
protected:
  void SetUp() override {
    // Save cout's buffer here
    originBuffer = cout.rdbuf();

    // Redirect cout to our stringstream buffer or any other ostream
    cout.rdbuf(coutBuffer.rdbuf());

    // prepare the list
    list.insertToFront("Hawkeye");
    list.insertToFront("Thor");
    list.insertToFront("Hulk");
    list.insertToFront("Black Widow");
    list.insertToFront("Iron Man");
    list.insertToFront("Captain America");
  }

  void TearDown() override { cout.rdbuf(originBuffer); }

  stringstream coutBuffer;
  streambuf   *originBuffer;

  LinkedList list;
};

TEST_F(CaptureCout, Print) {
  // WHEN
  list.printList();

  // THEN
  EXPECT_EQ("Captain America Iron Man Black Widow Hulk Thor Hawkeye\n",
            coutBuffer.str());
}

TEST_F(CaptureCout, PrintReverse) {
  // GIVEN
  LinkedList reverse = list;
  reverse.reverseList();

  // WHEN
  list.printReverse();
  string output = coutBuffer.str();

  coutBuffer.str("");
  reverse.printList();

  // THEN
  EXPECT_EQ(output, coutBuffer.str());
}

TEST_F(CaptureCout, PrintEmptyList) {
  // GIVEN
  LinkedList empty;

  // WHEN
  empty.printList();

  // THEN
  EXPECT_EQ("\n", coutBuffer.str());
}