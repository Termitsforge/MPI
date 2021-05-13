#include "mpi.h"
#include <stdio.h>
#include <malloc.h>

using namespace std;

int main(int *argc, char* argv[])
{
	int ProcNum, ProcRank, ProcSum;
	int rbuf[100], * sendbuf;
	// <программный код без использования MPI функций>
	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0) {
		for (int i = 0; i < 100; i++) {
			rbuf[i] = i + 10;
		}
	}
	sendbuf = (int*)malloc(ProcNum * 100 * sizeof(int));
	MPI_Scatter(sendbuf, 100, MPI_INT, rbuf, 100, MPI_INT, 0, MPI_COMM_WORLD);
	if (ProcRank != 0) {
		printf("---------------------------------------------------------\n");
		printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 100; i++) {
			rbuf[i] = i + 10;
			printf("i: %d; sendbuf: %d\n", i, *sendbuf);
		}
		printf("---------------------------------------------------------\n");
	}
	





	/*switch (ProcRank) {
		case 1:
			printf("rank: %d;\n", ProcRank);
			for (int i = 0; i < 25; i++) {
				rbuf[i] = i + 10;
				printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
			}
			break;
		case 2:
			printf("rank: %d;\n", ProcRank);
			for (int i = 25; i < 50; i++) {
				rbuf[i] = i + 10;
				printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
			}
			break;
			printf("rank: %d;\n", ProcRank);
		case 3:
			for (int i = 50; i < 75; i++) {
				rbuf[i] = i + 10;
				printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
			}
			break;
	}
	
	*/
	/*if(!ProcRank == 0){
		printf("rank: %d;\n", ProcRank);
		for (int i = 0; i < 25; i++) {
			rbuf[i] = i + 10;
			printf("i: %d; rbuf[i]: %d\n",i, rbuf[i]);
		}
	}*/
	// <программный код с использованием MPI функций>
	MPI_Finalize();
	/*printf("without MPI\n");
	for (int i = 0; i < 99; i++) {
		printf("i: %d; rbuf[i]: %d\n", i, rbuf[i]);
	}*/
	// <программный код без использования MPI функций>
	return 0;
}
