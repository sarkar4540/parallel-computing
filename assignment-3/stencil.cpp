#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    double thres = atof(argv[1]);
    int n;
    ifstream input("matrix");
    input >> n;

    int pid, pn;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    pn = pn - 1;
    double *arr[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            input >> arr[i][j];
        }
    }
    double current_threshold=INT32_MAX;
    if (pid == 0)
    {
        for (int i = 0; i < pn; i++)
        {
            int start = i * n / pn;
            int end = (i + 1) * n / pn;
            for (int j = start; j < end; j++)
            {
                MPI_Recv(arr[j],n,MPI_DOUBLE,i,1,MPI_COMM_WORLD,&status);
            }
        }
    }
    else
    {
        int start = (pid - 1) * n / pn;
        int end = (pid)*n / pn;
        for (int i = start; i < end; i++)
        {
            MPI_Send(arr[i],n,MPI_DOUBLE,pid,1,MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}