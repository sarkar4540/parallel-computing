#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    int m = atoi(argv[1]);
    int n;
    ifstream input("array");
    input >> n;
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        input >> arr[i];
    }

    int pid, pn;
    srand(time(0));
    MPI_Status status;
    MPI_Request request;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    if (pid == 0)
    {
        int np = 0;
        while (np == 0)
            np = rand() % pn;
        MPI_Bcast(&np, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Send(arr, n, MPI_INT, np, 1, MPI_COMM_WORLD);
        cout << "initial send: 0 to " << np << endl;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    for (int i = 0; i < m; i++)
    {
        int rp;
        MPI_Irecv(&rp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
        cout << "receive request: " << rp << endl;
        if (rp == pid)
        {
            MPI_Recv(arr, n, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            cout << "received by " << pid;
            int np = 0;
            while (np == pid)
                np = rand() % pn;
            MPI_Bcast(&np, 1, MPI_INT, pid, MPI_COMM_WORLD);
            MPI_Send(arr, n, MPI_INT, np, 1, MPI_COMM_WORLD);
            cout << "sent from " << pid << " to " << np << endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}