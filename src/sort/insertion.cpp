#include "insertion.h"

namespace CS32
{
    void InsertionSort::sort(int array[], int len)
    {
        if (len <= 0)
        {
            return;
        }

        int endOfSorted = 0;
        for (int startOfUnsort = 1; startOfUnsort < len; ++startOfUnsort)
        {
            int key = array[startOfUnsort];
            int current = startOfUnsort;
            while (current > 0 && array[current-1] > key)
            {
                array[current] = array[current-1];
                --current;
            }
            
            array[current] = key;
        }
    }
}
