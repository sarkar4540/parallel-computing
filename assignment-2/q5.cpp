#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    int n;
    ifstream input("vectors");
    input>>n;
    int v1[n],v2[n];
    for(int i=0;i<n;i++){
        input>>v1[i];
    }
    for(int i=0;i<n;i++){
        input>>v2[i];
    }

    int pid, pn;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    pn=pn-1;
    if (pid == 0)
    {
        int v3[n],temp;
        for(int i=0;i<n;i++){
            MPI_Recv(v3+i,1,MPI_INT,MPI_ANY_SOURCE,i,MPI_COMM_WORLD,&status);
            cout<<v3[i]<<" ";
        }
        cout<<endl;
    }
    else
    {
        int start=(pid-1)*n/pn;
        int end=(pid)*n/pn;
        for(int i=start;i<end;i++){
            int prod=v1[i]*v2[i];
            MPI_Send(&prod,1,MPI_INT,0,i,MPI_COMM_WORLD);
        }
        
    }
    MPI_Finalize();
    return 0;
}