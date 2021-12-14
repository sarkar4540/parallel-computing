#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " <n>"<<endl;
        return 1;
    }
    int n = atoi(argv[1]);
    ofstream output("vectors");
    output << n << endl;
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        output << rand() % 10000 << " ";
    }
    output<<endl;
    for (int i = 0; i < n; i++)
    {
        output << rand() % 10000 << " ";
    }
}