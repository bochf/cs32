#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "insertion.h"
#include "merge.h"
#include "tracker.h"

using namespace ::testing;
using namespace std;

namespace CS32
{
    template <typename ALG>
    class TestSortFixture : public Test
    {
    public:
        vector<vector<int>> m_data;
        ALG m_alg;

        void printData(const vector<int> &data)
        {
            for (int value : data)
            {
                cout << value << " ";
            }
            cout << endl;
        }

        static void SetUpTestCase()
        {
            srand(time(nullptr));
        }

        /// @brief generate a group of random numbers
        /// @param len the number the elements
        /// @param range the maxium value of the number, the value is in [0, range)
        void generateData(size_t len, size_t range)
        {
            m_data.emplace_back(vector<int>(len));
            for (size_t i = 0; i < len; ++i)
            {
                m_data.back()[i] = rand() % range;
            }
        }

        virtual void SetUp()
        {
            m_data.clear();

            // empty array
            m_data.push_back(vector<int>());

            // one element array
            m_data.push_back({1});

            // multiple elements array
            for (size_t i = 0; i < 5; ++i)
            {
                size_t len = rand() % 100 + 2; // number of elements in the array
                generateData(len, 100);
            }
        }

        virtual void TearDown()
        {
            for (const auto &array : m_data)
            {
                if (!array.empty())
                {
                    auto prev = array[0];
                    for (const auto &value : array)
                    {
                        EXPECT_LE(prev, value);
                        prev = value;
                    }
                }
            }
        }
    };

    using Algorithms = Types<MergeSort, InsertionSort>;
    TYPED_TEST_SUITE(TestSortFixture, Algorithms);

    TYPED_TEST(TestSortFixture, Basic)
    {
        for (auto &array : this->m_data)
        {
            NoopTracker tracker;
            this->m_alg.sort(array, tracker);
        }
    }

    TYPED_TEST(TestSortFixture, Performance)
    {
        const size_t len = 1 << 12;
        const size_t range = 10000;

        {
            this->m_data.clear();
            this->generateData(len, range);
            TimeTracker tracker(this->m_alg.name(), len);
            this->m_alg.sort(this->m_data.back(), tracker);
        }
    }
}