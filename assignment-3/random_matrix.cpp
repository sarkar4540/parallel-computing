#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <random>
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " <n>" << endl;
        return 1;
    }
    double lbound = 0;
    double ubound = 10;
    uniform_real_distribution<double> urd(lbound, ubound);
    default_random_engine re;
    int n = atoi(argv[1]);
    ofstream output("matrix");
    output << n << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            output << urd(re) << " ";
        }
        output << endl;
    }
}