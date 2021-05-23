#include "mpi.h"
#include <stdio.h>
#include <malloc.h>
#include <cmath>

using namespace std;

int Calculating_a_polynomial(int N, int x) {
	int sum = 0;
	for (int i = 0; i <= N; i++) {
		sum += pow(x,i);
	}
	return sum;
}


int main(int *argc, char* argv[])
{
	int ProcNum, ProcRank, GroupRank, GroupNum, Intermediate_amount = 0;
	long long SUM = 0;
	int n = 8, A = 12345678, B = 98765432;  // n - значность чисел, A и В - умножаемые числа
	int a, b, c, d, q, p, f;
	
	struct{
		int A;
		int B;
	} long_integer;

		
	// <программный код без использования MPI функций>
	MPI_Datatype longInt;
	int count = 2;
	MPI_Init(argc, &argv);
	MPI_Type_contiguous(count, MPI_INT, &longInt);
	MPI_Type_commit(&longInt);

	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_Comm Comm__;
	MPI_Group com_Karatsuba;
	MPI_Group world;
	MPI_Comm_group(MPI_COMM_WORLD, &world);

	int* Com_Ranks = new int[7]{0, 1, 2, 3, 4, 5, 6};

	MPI_Group_incl(world, 7, Com_Ranks, &com_Karatsuba);

	MPI_Group_size(com_Karatsuba, &GroupNum);
	MPI_Group_rank(com_Karatsuba, &GroupRank);
	MPI_Comm_create(MPI_COMM_WORLD, com_Karatsuba, &Comm__);

	if (GroupRank == 0) {
		a = A / 10000;
		b = A % 10000;
		c = B / 10000;
		d = B % 10000;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 1, 0, Comm__);

		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 2, 0, Comm__);

		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 3, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 1, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 2, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 3, 0, Comm__, MPI_STATUS_IGNORE);

		SUM = q * 100000000 + (p - q - f) * 10000 + f;
		printf("rank: %d; SUM : %llu; q :%d, p :%d, f :%d\n", GroupRank, SUM, q, p, f);
	}
	else if (GroupRank == 1 || GroupRank == 2 || GroupRank == 3) {
		MPI_Recv(&long_integer, 1, longInt, 0, 0, Comm__, MPI_STATUS_IGNORE);

		a = long_integer.A / 100;
		b = long_integer.A % 100;
		c = long_integer.B / 100;
		d = long_integer.B % 100;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 4, 0, Comm__);
		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 5, 0, Comm__);
		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 6, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 4, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 5, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 6, 0, Comm__, MPI_STATUS_IGNORE);

		Intermediate_amount = q * 10000 + (p - q - f) * 100 + f;
	
		MPI_Send(&Intermediate_amount, 1, MPI_INT, 0, 0, Comm__);
	}
	else if (GroupRank == 4 || GroupRank == 5 || GroupRank == 6) {
		for (int i = 1; i <= 3; i++) {
			MPI_Recv(&long_integer, 1, longInt, i, 0, Comm__, MPI_STATUS_IGNORE);
			int sum = long_integer.A * long_integer.B;
			MPI_Send(&sum, 1, MPI_INT, i, 0, Comm__);
		}
	}
	/*switch (GroupRank)
	{
	case 0:
		a = A / 10000;
		b = A % 10000;
		c = B / 10000;
		d = B % 10000;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 1, 0, Comm__);

		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 2, 0, Comm__);

		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 3, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 1, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 2, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 3, 0, Comm__, MPI_STATUS_IGNORE);

		SUM = q * 100000000 + (p - q - f) * 10000 + f;
		printf("rank: %d; SUM : %d; q :%d, p :%d, f :%d\n", GroupRank, SUM, q, p, f);
		break;
	case 1:
		MPI_Recv(&long_integer, 1, longInt, 0, 0, Comm__, MPI_STATUS_IGNORE);

		a = long_integer.A / 100;
		b = long_integer.A % 100;
		c = long_integer.B / 100;
		d = long_integer.B % 100;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 4, 0, Comm__);
		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 5, 0, Comm__);
		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 6, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 4, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 5, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 6, 0, Comm__, MPI_STATUS_IGNORE);

		Intermediate_amount = q * 10000 + (p - q - f) * 100 + f;
		printf("rank: %d; Intermediate_amount : %d\n", GroupRank, Intermediate_amount);
		MPI_Send(&Intermediate_amount, 1, MPI_INT, 0, 0, Comm__);
		break;
	case 2:
		MPI_Recv(&long_integer, 1, longInt, 0, 0, Comm__, MPI_STATUS_IGNORE);

		a = long_integer.A / 100;
		b = long_integer.A % 100;
		c = long_integer.B / 100;
		d = long_integer.B % 100;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 4, 0, Comm__);
		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 5, 0, Comm__);
		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 6, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 4, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 5, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 6, 0, Comm__, MPI_STATUS_IGNORE);

		Intermediate_amount = q * 10000 + (p - q - f) * 100 + f;
		printf("rank: %d; Intermediate_amount : %d\n", GroupRank, Intermediate_amount);
		MPI_Send(&Intermediate_amount, 1, MPI_INT, 0, 0, Comm__);
		break;
	case 3:
		MPI_Recv(&long_integer, 1, longInt, 0, 0, Comm__, MPI_STATUS_IGNORE);

		a = long_integer.A / 100;
		b = long_integer.A % 100;
		c = long_integer.B / 100;
		d = long_integer.B % 100;

		long_integer.A = a; long_integer.B = c;
		MPI_Send(&long_integer, 1, longInt, 4, 0, Comm__);
		long_integer.A = a + b; long_integer.B = c + d;
		MPI_Send(&long_integer, 1, longInt, 5, 0, Comm__);
		long_integer.A = b; long_integer.B = d;
		MPI_Send(&long_integer, 1, longInt, 6, 0, Comm__);

		MPI_Recv(&q, 1, MPI_INT, 4, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&p, 1, MPI_INT, 5, 0, Comm__, MPI_STATUS_IGNORE);
		MPI_Recv(&f, 1, MPI_INT, 6, 0, Comm__, MPI_STATUS_IGNORE);

		Intermediate_amount = q * 10000 + (p - q - f) * 100 + f;
		printf("rank: %d; Intermediate_amount : %d\n", GroupRank, Intermediate_amount);
		MPI_Send(&Intermediate_amount, 1, MPI_INT, 0, 0, Comm__);
		break;
	case 4:
		for (int i = 1; i <= 3; i++) {
			MPI_Recv(&long_integer, 1, longInt, i, 0, Comm__, MPI_STATUS_IGNORE);
			int sum = long_integer.A * long_integer.B;
			MPI_Send(&sum, 1, MPI_INT, i, 0, Comm__);
		}
		break;
	case 5:
		for (int i = 1; i <= 3; i++) {
			MPI_Recv(&long_integer, 1, longInt, i, 0, Comm__, MPI_STATUS_IGNORE);
			int sum = long_integer.A * long_integer.B;
			printf("rank: %d; sum : %d\n", GroupRank, sum);
			MPI_Send(&sum, 1, MPI_INT, i, 0, Comm__);
		}
		break;
	case 6:
		for (int i = 1; i <= 3; i++) {
			MPI_Recv(&long_integer, 1, longInt, i, 0, Comm__, MPI_STATUS_IGNORE);
			int sum = long_integer.A * long_integer.B;
			
			MPI_Send(&sum, 1, MPI_INT, i, 0, Comm__);
		}
		break;
	default:
		break;
	}*/
	
		// Отправка данных
		//for (int i = 1; i < ProcNum; i++) {
		//	pol_info.N = N; pol_info.x = x; pol_info.A = num_multiplications[i];
		//	MPI_Send(&pol_info, 1, polinom, i, 0, MPI_COMM_WORLD);
		//}
		//// Вычисление своей части
		//pol_info.N = N; pol_info.x = x; pol_info.A = num_multiplications[0];
		//printf("rank: %d; pol_info.N: %d; pol_info.x: %d pol_info.A: %d\n", ProcRank, pol_info.N, pol_info.x, pol_info.A);
		//sum_pol = Calculating_a_polynomial(pol_info.N, pol_info.x);
		//if (A != 0) {
		//	result[0] = 1;
		//	for (int i = 0; i < pol_info.A; i++) {
		//		result[0] *= sum_pol;
		//	}
		//}
		//else result[0] = 0;
		//// Получение данных от других процессов
		//MPI_Recv(&result[1], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//MPI_Recv(&result[2], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//MPI_Recv(&result[3], 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//for (int i = 0; i < 4; i++) {
		//	SUM *= result[i];
		//}
	
		/*MPI_Recv(&pol_info, 1, polinom, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("rank: %d; pol_info.N: %d; pol_info.x: %d pol_info.A: %d\n", ProcRank, pol_info.N, pol_info.x, pol_info.A);
		sum_pol = Calculating_a_polynomial(pol_info.N, pol_info.x);
		int res;
		if (A != 0) {
			res = 1;
			for (int i = 0; i < pol_info.A; i++) {
				res *= sum_pol;
			}
		}
		else res = 0;
		MPI_Send(&res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);*/

	
	MPI_Group_free(&com_Karatsuba);
	// <программный код с использованием MPI функций>
	MPI_Finalize();
	// <программный код без использования MPI функций>
	return 0;
}
