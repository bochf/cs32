#include "sorts.cpp"

#include <gtest/gtest.h>
#include <map>
#include <vector>

using namespace ::testing;
using namespace std;

#define K        *1000UL
#define M        *1000000UL
#define MAX_SIZE 1 M

class TestData {
public:
  static TestData &instance() {
    if (!s_instance) {
      s_instance = new TestData();
    }
    return *s_instance;
  };

  void fillData(vector<Sensor> &sensors, size_t size) const {
    sensors.clear();
    for (size_t i = 0; i < size; ++i) {
      sensors.emplace_back(g_sensors[i]);
    }
  }

  void init() {
    vector<IdType> ids;
    for (size_t j = 0; j < MAX_SIZE; j++) {
      ids.push_back(IdType(j));
    }

    std::random_device rd;
    std::mt19937       g(rd());
    std::shuffle(ids.begin(), ids.end(), g);

    // Create a bunch of Sensors
    vector<Sensor> sensors;
    for (size_t k = 0; k < ids.size(); k++) {
      g_sensors.emplace_back(ids[k]);
    }

    for (size_t n = 100 K; n <= MAX_SIZE; n += 20 K) {
      g_stlDataSizes.push_back(n);
    }
    for (size_t n = 3 K; n <= 30 K; n += 600) {
      g_InsertionDataSizes.push_back(n);
    }
  }

  vector<size_t> g_stlDataSizes;
  vector<size_t> g_InsertionDataSizes;

private:
  static TestData *s_instance;
  vector<Sensor>   g_sensors;
};

TestData *TestData::s_instance = nullptr;

class TestEnvironment : public Environment {
public:
  void SetUp() override { TestData::instance().init(); };
};

class InsertionSort : public TestWithParam<int> {
protected:
  void SetUp() final {
    size_t size = TestData::instance().g_InsertionDataSizes[GetParam()];
    TestData::instance().fillData(m_sensors, size);
  }

  void TearDown() final { m_sensors.clear(); }

  vector<Sensor> m_sensors;
};

class StlSort : public TestWithParam<int> {
protected:
  void SetUp() final {
    size_t size = TestData::instance().g_stlDataSizes[GetParam()];
    TestData::instance().fillData(m_sensors, size);
  }

  void TearDown() final { m_sensors.clear(); }

  vector<Sensor> m_sensors;
};

TEST_P(StlSort, testStlSort) {
  sort(m_sensors.begin(), m_sensors.end(), compareSensor);
}

TEST_P(InsertionSort, testPtrInsertionSort) { insertion_sort_ptr(m_sensors); }

TEST_P(InsertionSort, testInsertionSort) {
  insertion_sort(m_sensors, compareSensor);
}

INSTANTIATE_TEST_SUITE_P(InitiateArraySize,
                         StlSort,
                         Range(0, 40),
                         PrintToStringParamName());

INSTANTIATE_TEST_SUITE_P(InitiateArraySize,
                         InsertionSort,
                         Range(0, 40),
                         PrintToStringParamName());

int main(int argc, char *argv[]) {
  InitGoogleTest(&argc, argv);
  AddGlobalTestEnvironment(new TestEnvironment);
  return RUN_ALL_TESTS();
}