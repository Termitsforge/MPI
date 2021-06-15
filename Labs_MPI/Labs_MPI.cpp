#include <iostream>
#include <mpi.h>
using std::cout;
using std::cin;
using std::endl;

int* polyMultipl(int* pMax, int sizeMax, int* pMin, int sizeMin);
int* CreateNumber(int N);
void out(int N, int* p);
int main(int argc, char** argv) {
    int N = 12;//степень числа
    int n = N / 4;
    int ProcRank,//ранг процесса
        world_size;//количество запущенных процессов

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //////////////////////////////////// 
    MPI_Datatype type;
    MPI_Datatype typeDoub;
    MPI_Datatype typeN;
    MPI_Type_contiguous(n, MPI_INT, &type);
    MPI_Type_contiguous(N, MPI_INT, &typeN);
    MPI_Type_contiguous(2, typeN, &typeDoub);
    MPI_Type_commit(&type);
    MPI_Type_commit(&typeDoub);
    MPI_Type_commit(&typeN);

    int index[] = { 1,2,3,4,8 };
    int edges[] = { 4,4,4,4,1,2,3,0};
    MPI_Comm StarComm;
    MPI_Graph_create(MPI_COMM_WORLD, 5, index, edges, 1, &StarComm);
    int neighbours_count;
    MPI_Graph_neighbors_count(StarComm, ProcRank, &neighbours_count);
    int* neighbours = new int[neighbours_count];
    MPI_Graph_neighbors(StarComm, ProcRank, neighbours_count, neighbours);


    int* result=new int[N * 2];
    int* TwoNum;
    int* OneNum;
    int* countOne = new int[N * 2];
    int* countTwo = new int[N * 2];
    int* countThree = new int[N * 2];
    int* countFour = new int[N * 2];
    if (ProcRank == 4)
    {
        OneNum = CreateNumber(N);
        cout << "One Num : ";
        out(N, OneNum);
        TwoNum = CreateNumber(N);
        cout << "Two Num : ";
        out(N, TwoNum);
        for (int i = 0; i < 4; i++) {
            int first[] = { OneNum[0 + 3 * i],  OneNum[1 + 3 * i],  OneNum[2 + 3 * i] };
            MPI_Send(first, 1, type, i, 0, MPI_COMM_WORLD);
            MPI_Send(TwoNum, 1, typeN, i, 0, MPI_COMM_WORLD);
        }
        MPI_Recv(countOne, 1, typeDoub, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(countTwo, 1, typeDoub, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(countThree, 1, typeDoub, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(countFour, 1, typeDoub, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for (int i = 0; i < N * 2; i++) {
            result[i] = countOne[i] + countTwo[i] + countThree[i] + countFour[i];
        }
        for (int i = 0; i < N * 2; i++)
        {
            if (result[i] > 9) {
                int ost = result[i] / 10;
                result[i] = result[i] % 10;
                result[i + 1] += ost;
            }
        }
        cout << "Result :" << endl;
        for (int i = N*2 - 1; i >= 0; i--) {
            cout << result[i];
        }
        cout << endl;
    }
    else {
        int first[3];
        int* second = new int[N];
        MPI_Recv(first, 1, type, 4, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(second, 1, typeDoub, 4, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       
        int* One = new int[N];
        for (int i = 0; i < N; i++)
        {
            One[i] = 0;
        }
       
        int i = 0;
        One[i+ProcRank * 3  ] = first[0];
        One[i+ProcRank * 3+1] = first[1];
        One[i+ProcRank * 3+2] = first[2];
        
        int* res;
        res = polyMultipl(second, N, One, N);
       MPI_Send(res, 1, typeDoub, 4, 0, MPI_COMM_WORLD);
    }

    MPI_Type_free(&type);
    MPI_Type_free(&typeDoub);
    MPI_Finalize();
    return 0;
}

int* polyMultipl(int* pMax, int sizeMax, int* pMin, int sizeMin)
{
    int* p = new int[sizeMax];
    int x = sizeMax + sizeMin;
    int* a = new int[x+1];
    int* b = new int[x + 1];
    for (int i = 0; i < x; i++)
    {
        a[i] = 0;
    }

    for (int j = 0; j < sizeMin; j++)
    {
        for (int i = 0; i < sizeMax; i++)
        {
            p[i] = pMax[i] * pMin[j];
        }

        for (int i = 0; i < sizeMax; i++)
        {
            a[i + j] = a[i + j] + p[i];
        }
        
    }
    for (int i = 0; i <x; i++)
    {
        if (a[i] > 9) {
            int ost = a[i] / 10;
            a[i] = a[i] % 10;
            a[i + 1] += ost;
        }
    }
    delete[] p;
    return a;
}

int* CreateNumber(int N)
{
    int* a = new int[N];
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 8 + 1;
    }
    return a;
}

void out(int N, int* p)
{
    cout << endl;
    for (int i = N - 1; i >= 0; i--)
    {
        cout << p[i];
    }
    cout << endl;
}