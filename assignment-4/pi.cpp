#include <iostream>
#include <stdio.h>
#include <mpi/mpi.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char *argv[])
{
	int n, rank, size, i;
	double pi_value = 3.141592653589793238462643;
	double pi2, pi1, temp, sum;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		cout << "Enter n:" << endl;
		cin >> n;
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (n < 1)
		return EXIT_FAILURE;

	temp = 1.0 / (double)n;
	sum = 0.0;
#pragma omp parallel for reduction(+ \: sum)
	for (i = rank + 1; i <= n; i += size)
	{
		double x = temp * ((double)i - 0.5);
		sum += (4.0 / (1.0 + x * x));
	}
	pi2 = temp * sum;

	MPI_Reduce(&pi2, &pi1, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Finalize();

	if (rank == 0)
	{
		cout << "pi value found is " << pi1 << "\nError: " << fabs(pi1 - pi_value) << endl;
	}
	return EXIT_SUCCESS;
}
