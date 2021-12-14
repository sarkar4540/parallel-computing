#include <iostream>
#include <cstdlib>
#include <mpi/mpi.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    int n;
    ifstream input("array");
    input>>n;
    int arr[n];
    for(int i=0;i<n;i++){
        input>>arr[i];
    }

    int pid, pn;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    pn=pn-1;
    if (pid == 0)
    {
        int sum=0,temp;
        for(int i=1;i<=pn;i++){
            MPI_Recv(&temp,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
            sum=sum+temp;
        }
        cout<<sum<<endl;
    }
    else
    {
        int start=(pid-1)*n/pn;
        int end=(pid)*n/pn;
        int sum=0;
        for(int i=start;i<end;i++){
            sum+=arr[i];
        }
        MPI_Send(&sum,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}