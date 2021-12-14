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
    srand(time(0));
    for(int i=0;i<n;i++){
        arr[i]=rand();
    }

    int pid, pn;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);
    
    cout<<"Before sending PID ["<<pid<<"] : ";
    for(int i:arr){
        cout<<i<<" ";
    }
    cout<<endl;
    
    MPI_Send(arr,n,MPI_INT,(pid+1==pn)?0:pid+1,0,MPI_COMM_WORLD);
    MPI_Recv(arr,n,MPI_INT,pid==0?pn-1:pid-1,0,MPI_COMM_WORLD,&status);

    cout<<"After receiving PID ["<<pid<<"] : ";
    for(int i:arr){
        cout<<i<<" ";
    }
    cout<<endl;

    MPI_Finalize();
    return 0;
}