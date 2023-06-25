#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>

#include "insertion.h"

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

        virtual void SetUp()
        {
            m_data.clear();

            // empty array
            m_data.push_back(vector<int>());

            // one element array
            m_data.push_back({1});

            // multiple elements array
            default_random_engine generator;
            uniform_int_distribution<int> distribution(1, 100);
            vector<int> tmp;
            for (size_t i = 0; i < 10; ++i)
            {
                tmp.push_back(distribution(generator));
            }
            m_data.push_back(tmp);
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
            cout << "Test sort by ";
            printData(array);
            algorithm.sort(array.data(), array.size());
            cout << "Sort result is ";
            printData(array);
        }
    }

}