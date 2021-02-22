
//#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
#define ITT 1000000000ULL 

int main(int argc, char** argv) {

	unsigned long long count;
	int size, rank;
	unsigned long long  res = 0;
	unsigned long long  sum = 0;
	unsigned long long  proc_sum = 0;

	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	unsigned long long step = ITT / size;

	for (int i = rank; i < ITT + 1; i += size)
	{
		sum += 1;
	}


	if (size > 1) {
		for (int i = 0; i < size; i++) {
			if (rank == i)
			{
				if (i % 2 != 0) {
					MPI_Send(&sum, 1, MPI_UNSIGNED_LONG_LONG, i - 1, 7, MPI_COMM_WORLD);
				}

				if (i % 2 == 0) {
					MPI_Recv(&proc_sum, 1, MPI_UNSIGNED_LONG_LONG, i + 1, 7, MPI_COMM_WORLD, &status);
					sum += proc_sum;
				}
			}
		}

		int n = 2;
		while (n != size) {
			for (int i = 0; i < size; i += n * 2) {

				if (rank == i + n) {
					MPI_Send(&sum, 1, MPI_UNSIGNED_LONG_LONG, i, 7, MPI_COMM_WORLD);
				}
				if (rank == i) {
					MPI_Recv(&proc_sum, 1, MPI_UNSIGNED_LONG_LONG, i + n, 7, MPI_COMM_WORLD, &status);
					sum += proc_sum;
				}
			}
			n *= 2;
		}

	}

	MPI_Finalize();

	if (rank == 0)

	{
		printf("\nresult: ");
		cout << sum;
		printf("\nRun time: %f", clock() / 1000.0);
	}
}