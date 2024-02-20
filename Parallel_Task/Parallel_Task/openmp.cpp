#include <iostream>
#include <omp.h>
#include <chrono>
using namespace std::chrono;
using namespace std;


int man()
{
    auto start = high_resolution_clock::now();
    int a[30][30], b[30][30], c[30][30];
    

    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            a[i][j] =i+ 1;
            b[i][j] =j+ 1;
        }
    }


#pragma omp parallel for num_threads(3)
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 30; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }

        }
    }


    auto stop = high_resolution_clock::now();
    cout << "\n\n";
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            cout << (unsigned)c[i][j] << "  ";
            do
            {
                c[i][j] *= 10;
                cout << " ";
            } while (c[i][j] < 100000);
        }
        cout << "\n \n";

    }
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "time taken in microseconds= " << duration.count();
    return 0;
}

