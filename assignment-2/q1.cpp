#include<iostream>
#include<cstdlib>
#include<mpi/mpi.h>

using namespace std;

int main(int argc, char** argv){
    if(argc<2){
        cout<<"usage: "<<argv[0]<<" n "<<endl;
        return 1;
    }
    int n=atoi(argv[1]);
    cout<<n<<endl;
    return 0;
}