#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>
#include <omp.h>

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
    double current_diff = INT32_MAX;
    int m = 0;
    while (current_diff > thres && m<100)
    {
        m++;
        if (pid == 0)
        {
            current_diff = 0;
            for (int i = 1; i < pn; i++)
            {
                double diff_val;
                MPI_Recv(&diff_val, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                current_diff = max<double>(current_diff, diff_val);
            }
        }
        else
        {
            int start = (pid - 1) * n / pn;
            int end = (pid)*n / pn;
            double max_diff = 0;
            for (int i = start; i < end; i++)
            {
#pragma omp parallel for shared(arr, max_diff)
                for (int j = 0; j < n; j++)
                {
                    double sum = arr[i][j];
                    int count = 1;
                    if (i > 0)
                    {
                        sum += arr[i - 1][j];
                        count++;
                    }
                    if (j > 0)
                    {
                        sum += arr[i][j - 1];
                        count++;
                    }
                    if (i < n - 1)
                    {
                        sum += arr[i + 1][j];
                        count++;
                    }
                    if (j < n - 1)
                    {
                        sum += arr[i][j + 1];
                        count++;
                    }
                    double new_val = sum / count;
                    max_diff = max<double>(max_diff, new_val - arr[i][j]);
                    arr[i][j] = new_val;
                }
            }
            MPI_Send(&max_diff, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
        MPI_Bcast(&current_diff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (int i = 0; i < n; i++)
        {
            MPI_Bcast(arr[i], n, MPI_DOUBLE, i * pn / n + 1, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if(pid==0){
        cout<<m<<endl;
    }
    MPI_Finalize();
    return 0;
}