#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <mpi/mpi.h>

using namespace std;

void iittpbarrier(int, int); 

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);               
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    printf("Before Barrier  from process %d \n", rank);
    sleep(10); 

    iittpbarrier(rank, size);

    sleep(10); 
    printf("After  Barrier  from process %d \n", rank);

    MPI_Finalize();
    return 0;
}
void iittpbarrier(int pid, int size)
{

    double logVal = log(size) / log(2);

    int k = logVal;
    int i = 0;
    int j = 0;
    int iteration = 0;
    for (i = k - 1; i >= 0; i--)
    {
        iteration = 2 - i;
        for (j = pow(2, i); j < pow(2, i + 1); j++)
        {

            if (pid == j)
            {
                MPI_Send(&iteration, 1, MPI_INT, j - (int)pow(2, i), iteration, MPI_COMM_WORLD);
            }

            if (pid == j - (int)pow(2, i))
            {

                MPI_Status status;
                int data;
                MPI_Recv(&data, 1, MPI_INT, j, iteration, MPI_COMM_WORLD, &status);
            }
        }
    }

    i = 0;
    j = 0;
    iteration = 0;

    for (i = 0; i <= k - 1; i++)
    {
        iteration = i;
        for (j = 0; j <= pow(2, i) - 1; j++)
        {

            if (pid == j)
            {
                MPI_Send(&iteration, 1, MPI_INT, j + (int)pow(2, i), iteration, MPI_COMM_WORLD);
            }

            if (pid == j + (int)pow(2, i))
            {

                MPI_Status status;
                int data;
                MPI_Recv(&data, 1, MPI_INT, j, iteration, MPI_COMM_WORLD, &status);
                printf("\n\n receiving message from %d to %d and message is %d", j, j + (int)pow(2, i), data);
            }
        }
    }
}