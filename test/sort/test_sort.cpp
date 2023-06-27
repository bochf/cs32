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
    class TestSort : public Test
    {
    protected:
        vector<vector<int>> m_data;

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

    TEST_F(TestSort, TestInsertion)
    {
        InsertionSort algorithm;

        for (auto &array : m_data)
        {
            NoopTracker tracker;
            algorithm.sort(array, tracker);
        }
    }

    TEST_F(TestSort, TestMerge)
    {
        MergeSort algorithm;
        for (auto &array : m_data)
        {
            NoopTracker tracker;
            algorithm.sort(array, tracker);
        }
    }

    TEST_F(TestSort, PerformanceTest)
    {
        const size_t len = 1 << 10;
        const size_t range = 10000;

        {
            m_data.clear();
            generateData(len, range);
            InsertionSort algorithm;
            TimeTracker tracker("InsertionSort", len);
            algorithm.sort(m_data.back(), tracker);
        }

        {
            m_data.clear();
            generateData(len, range);
            MergeSort algorithm;
            TimeTracker tracker("MergeSort", len);
            algorithm.sort(m_data.back(), tracker);
        }
    }
}