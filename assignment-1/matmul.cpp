#include <iostream>
#include <map>
#include <ctime>
#include <time.h>
#include <cstring>
#include <iomanip>

using namespace std;

typedef struct{
    map<int, map<int, int>> res;
    int count;
} MapMultiplyResult;

void fillMatrixSparseRandomValues(int **array, int m, int n, float sp)
{
    int filled = 0;
    while (((float)filled) / (m * n) < sp)
    {
        int row = rand() % m;
        int col = rand() % n;
        if (array[row][col] == 0)
        {
            array[row][col] = rand()%10000;
            filled++;
        }
    }
}

void printMatrix(int **array, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << setw(14) << array[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

map<int, map<int, int>> matrixToMap(int **array, int m, int n)
{
    map<int, map<int, int>> dense;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (array[i][j] != 0)
            {
                dense[i][j] = array[i][j];
            }
        }
    }
    return dense;
}

void printMap(map<int, map<int, int>> m)
{
    cout << "{" << endl;
    for (auto col : m)
    {
        cout << "\t" << col.first << ":{" << endl;
        for (auto elem : col.second)
        {
            cout << "\t\t" << elem.first << ":" << elem.second << "," << endl;
        }
        cout << "\t}," << endl;
    }
    cout << "}" << endl
         << endl;
}

int matrixMultiply(int **A, int **B, int **C, int m, int n, int p)
{
    int count = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            for (int k = 0; k < n; k++)
            {
                count++;
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return count;
}

MapMultiplyResult mapMultiply(map<int, map<int, int>> A, map<int, map<int, int>> B)
{
    MapMultiplyResult result;
    result.count=0;
    for (auto it = A.begin(); it != A.end(); ++it)
    {

        int n_row = it->first;

        for (auto i = it->second.begin(); i != it->second.end(); ++i)
        {

            int col = i->first;
            int val = i->second;

            for (auto j = B[col].begin(); j != B[col].end(); ++j)
            {
                int n_col = j->first;
                int n_val = val * j->second;
                result.count++;
                result.res[n_row][n_col] += n_val;
            }
        }
    }
    return result;
}

int main(int argc, char *argv[])
{

    if (argc < 11)
    {
        cout << "Usage: " << argv[0] << " -m <val> -n <val> -p <val> -spA <val> -spB <val> [-d]" << endl;
        return -1;
    }

    int m, n, p, d = 0;
    float spA, spB;
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == "-m")
            m = atoi(argv[++i]);
        else if (string(argv[i]) == "-n")
            n = atoi(argv[++i]);
        else if (string(argv[i]) == "-p")
            p = atoi(argv[++i]);
        else if (string(argv[i]) == "-spA")
            spA = atof(argv[++i]);
        else if (string(argv[i]) == "-spB")
            spB = atof(argv[++i]);
        else if (string(argv[i]) == "-d")
            d = 1;
    }

    int **A = new int *[m], **B = new int *[n], **C = new int *[m];
    for (int i = 0; i < m; i++)
    {
        A[i] = new int[n];
        memset(A[i], 0, sizeof(A[i]));
    }
    for (int i = 0; i < n; i++)
    {
        B[i] = new int[p];
        memset(B[i], 0, sizeof(B[i]));
    }
    for (int i = 0; i < m; i++)
    {
        C[i] = new int[p];
        memset(C[i], 0, sizeof(C[i]));
    }

    srand(time(0));
    fillMatrixSparseRandomValues(A, m, n, spA);
    fillMatrixSparseRandomValues(B, n, p, spB);

    map<int, map<int, int>> Ad = matrixToMap(A, m, n);
    map<int, map<int, int>> Bd = matrixToMap(B, n, p);

    clock_t start1 = clock();
    int count1 = matrixMultiply(A, B, C, m, n, p);
    clock_t end1 = clock();

    clock_t start2 = clock();
    MapMultiplyResult rCd= mapMultiply(Ad, Bd);
    clock_t end2 = clock();

    if (d)
    {
        cout << "Sparse A (" << m << "x" << n << ")\n"
             << endl;
        printMatrix(A, m, n);
        cout << "Sparse B (" << n << "x" << p << ")\n"
             << endl;
        printMatrix(B, n, p);
        cout << "Sparse C (" << m << "x" << p << ")\n"
             << endl;
        printMatrix(C, m, p);

        cout << "Dense A\n"
             << endl;
        printMap(Ad);
        cout << "Dense B\n"
             << endl;
        printMap(Bd);
        cout << "Dense C\n"
             << endl;
        printMap(rCd.res);
        cout << endl;
    }
    for (int i = 0; i < m; i++)
    {
        delete[] A[i];
    }
    delete[] A;
    for (int i = 0; i < n; i++)
    {
        delete[] B[i];
    }
    delete[] B;
    for (int i = 0; i < m; i++)
    {
        delete[] C[i];
    }
    delete[] C;
    cout << (end1 - start1) << "," << count1 << ","<< (end2 - start2) << "," << rCd.count <<endl;
    return 0;
}
