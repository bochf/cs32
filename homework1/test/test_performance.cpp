///
/// test LinkedList with large number of nodes
///

#include "LinkedList.h"

#include <gtest/gtest.h>

using namespace ::testing;

class Performance : public Test {
public:
  static void SetUpTestSuite() {
    const int len = 1 << 24;
    for (int i = 0; i < len; ++i) {
      sourceList.insertToFront("source");
      targetList.insertToFront("target");
    }
  }

  static LinkedList sourceList;
  static LinkedList targetList;
  static LinkedList newList;
};

LinkedList Performance::sourceList;
LinkedList Performance::targetList;
LinkedList Performance::newList;

TEST_F(Performance, LongListAssignment) { sourceList = targetList; }
TEST_F(Performance, CopyLongListToNew) { newList = targetList; }
