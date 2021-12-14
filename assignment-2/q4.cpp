#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    int o = atoi(argv[1]), n = atoi(argv[2]), b = atoi(argv[3]);

    int pid, pn;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    pn = pn - 1;
    if (pid == 0)
    {
        int sum = 0, temp;
        for (int i = 1; i <= pn; i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            sum = sum + temp;
        }
        cout << sum << endl;
    }
    else
    {
        switch (o)
        {
        case 1: //block distribution
        {
            int start = (pid - 1) * n / pn;
            int end = (pid)*n / pn;
            int arr[end - start];
            int sum = 0;
            for (int i = start; i < end; i++)
            {
                arr[i - start] = i;
                sum += arr[i - start];
            }
            MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        break;
        case 2: //cyclic distribution
        {
            int arr[n / pn + 1];
            int sum = 0;
            int d = 0;
            for (int i = pid - 1; i < n; i += pn)
            {
                arr[d] = i;
                sum += arr[d];
                d++;
            }
            MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        break;
        case 3: //block cyclic distribution
        {
            int arr[n / pn + 1];
            int sum = 0;
            int d = 0;
            for (int i = (pid - 1) * b; i < n; i += pn * b)
            {
                for (int j = 0; j < b; j++)
                {
                    arr[d] = i + j;
                    sum += arr[d];
                    d++;
                }
            }
            MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        break;
        }
    }
    MPI_Finalize();
    return 0;
}