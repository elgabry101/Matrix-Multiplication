#include <iostream>
#include <mpi.h>
#include <chrono>
using namespace std;
using namespace std::chrono;


int main(int argc, char** argv)
{
    auto start = high_resolution_clock::now();
    int rank, size, i, j, k;
    unsigned int a[30][30], b[30][30], c[30][30];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sizes[] = { 15,15};
    int distances[] = { 0,15 };

    unsigned int a_priv[30][15], b_priv[15][30], c_priv[30][30];

    if (rank == 0) 
    {
        for (i = 0; i < 30; i++)
        {
            for (j = 0; j < 30; j++)
            {
                a[i][j] = i+1;
                b[i][j] = j+1;
            }
        }

    }

    MPI_Scatter(b,450, MPI_INT, &b_priv,15 * 30, MPI_INT, 0, MPI_COMM_WORLD);
    for (int s = 0; s < 30; s++)
    {
        MPI_Scatterv(&a[s][0], sizes, distances, MPI_INT, &a_priv[s][0], 15, MPI_INT, 0, MPI_COMM_WORLD);
    }


    for (i = 0; i < 30; i++)
    {
        for (j = 0; j < 30; j++)
        {
            c_priv[i][j] = 0;
            for (k = 0; k < 15; k++)
                c_priv[i][j] += a_priv[i][k] * b_priv[k][j];
        }

    }
    MPI_Reduce(&c_priv,c , 30*30, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    

    if (rank == 0) {

        auto stop = high_resolution_clock::now();
        cout << "\nMultiplication of two matrices: \n";
        for (i = 0; i < 30; i++)
        {
            for (j = 0; j < 30; j++)
            {
                cout << (unsigned)c[i][j] << "  ";
                do
                {
                    c[i][j] *= 10;
                    cout << " ";
                } while (c[i][j] < 100000);
            }
            cout<< "\n \n";
        }
        
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "time taken in microseconds= " << duration.count();
    }


    MPI_Finalize();
    return 0;
}


