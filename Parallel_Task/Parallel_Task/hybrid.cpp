#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int min(int argc, char** argv)
{

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto start = high_resolution_clock::now();

    int a[30][30], b[30][30], c[30][30];
    int a_priv[30][15], b_priv[15][30], c_priv[30][30];
    int s1[] = { 450,450 };
    int s2[] = { 15,15 };
    int d1[] = { 0,450 };
    int d2[] = { 0,15 };

  

    if (rank == 0) 
    {
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                a[i][j] = i+1;
                b[i][j] = j+1;
            }
        }

    }

    MPI_Scatterv(&b, s1, d1, MPI_INT, &b_priv, 15 * 30, MPI_INT, 0, MPI_COMM_WORLD);
    for (int s = 0; s < 30; s++)
    {
        MPI_Scatterv(&a[s][0], s2, d2, MPI_INT, &a_priv[s][0], 15, MPI_INT, 0, MPI_COMM_WORLD);
    }


#pragma omp parallel for num_threads(2)
    for (int i = 0; i < 30; i++)
    {

        for (int j = 0; j < 30; j++)
        {
            c_priv[i][j] = 0;
            for (int k = 0; k < 15; k++)
            {
                c_priv[i][j] += a_priv[i][k] * b_priv[k][j];

            }

        }
    }
    


    MPI_Reduce(&c_priv, c, 30 * 30, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) 
    {
        auto stop = high_resolution_clock::now();
        cout << "\n\n";
        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                cout << c[i][j] << "  ";
                do
                {
                    c[i][j] *=10 ;
                    cout << " ";
                } while (c[i][j] < 100000);

            }
            cout << "\n\n";
        }

        auto duration = duration_cast<microseconds>(stop - start);
        cout << "time taken in microseconds= " << duration.count();
    }


    MPI_Finalize();
    return 0;
}
