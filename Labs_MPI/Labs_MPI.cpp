#include "mpi.h"
#include <stdio.h>
#include <malloc.h>

using namespace std;

int main(int *argc, char* argv[])
{
	int ProcNum, ProcRank, ProcSum, result = 0;
	int rbuf[100], * sendbuf, *resultbuf;
	// <программный код без использования MPI функций>
	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	resultbuf = (int*)malloc(ProcNum * 100 * sizeof(int));
	sendbuf = (int*)malloc(ProcNum * 100 * sizeof(int));
	if (ProcRank == 0) {
		for (int i = 0; i < 100; i++) {
			sendbuf[i] = i;
		}
	}
	MPI_Scatter(sendbuf, 25, MPI_INT, rbuf, 25, MPI_INT, 0, MPI_COMM_WORLD);
	switch (ProcRank) {
	case 0:
		//printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 25; i++) {
			rbuf[i] += ProcRank;
			//printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
		}
		break;
	case 1:
		//printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 25; i++) {
			rbuf[i] += ProcRank;
			//printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
		}
		break;
	case 2:
		//printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 25; i++) {
			rbuf[i] += ProcRank;
			//printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
		}
		break;
	case 3:
		//printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 25; i++) {
			rbuf[i] += ProcRank;
			//printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
		}
		break;
	}

	MPI_Gather(rbuf, 25, MPI_INT, resultbuf, 25, MPI_INT, 0, MPI_COMM_WORLD);

	if (ProcRank == 0) {
		printf("------------------------------\n");
		for (int i = 0; i < 50; i++) {
			printf("i: %d; resultbuf[i]: %d \t i: %d; resultbuf[i]: %d\n", i, resultbuf[i], i+50, resultbuf[i+50]);
			if (i == 24 || i == 49 || i == 74 || i == 99) {
				printf("-------------\n");
			}
		}
	}

	// <программный код с использованием MPI функций>
	MPI_Finalize();
	
	// <программный код без использования MPI функций>
	return 0;
}
