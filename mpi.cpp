
#include <iostream>
#include "mpi.h"
#define N 2000
#define FIRST_THREAD 0

int* matrix_multiplication(int*, int*, int, int);

const int FIRST_TAG = 1, SECOND_TAG = 2;
using namespace std;


int main(int *argc, char **argv )
{
	int *matrix1, *matrix2, * mult_matrix, *res_matrix;
	int thread, thread_size;
	double start, end;
	matrix1 = new int[N * N];
	matrix2 = new int[N * N];
	res_matrix = new int[N * N];
	// Инициализируем работу MPI
	MPI_Init(argc, &argv);
	// Получаем номер конкретного процесса на котором запущена программа
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);
	// Получаем количество запущенных процессов
	MPI_Comm_size(MPI_COMM_WORLD, &thread_size);
	if (thread == FIRST_THREAD)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				matrix1[i * N + j] = rand() % N;
				matrix2[i * N + j] = rand() % N;
			}
		}

		start = MPI_Wtime();
		for (int to_thread = 1; to_thread < thread_size; to_thread++) 
		{
			MPI_Send(matrix1, N * N, MPI_INT, to_thread, FIRST_TAG, MPI_COMM_WORLD);
			MPI_Send(matrix2, N * N, MPI_INT, to_thread, SECOND_TAG, MPI_COMM_WORLD);
		}
	}
	else 
	{
		MPI_Recv(matrix1, N * N, MPI_INT, FIRST_THREAD, FIRST_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(matrix2, N * N, MPI_INT, FIRST_THREAD, SECOND_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	
	mult_matrix = matrix_multiplication(matrix1, matrix2, thread, thread_size);

	MPI_Reduce(mult_matrix, res_matrix, N * N, MPI_INT, MPI_SUM, FIRST_THREAD, MPI_COMM_WORLD);

	end = MPI_Wtime();
	if (thread == FIRST_THREAD) {

		cout << "Count thread = " << thread_size << ", execution time = " << end - start << "s\n";
	}

	MPI_Finalize();
	

	return 0;

}

int* matrix_multiplication(int* matrix1, int* matrix2, int thread, int thread_size)
{
	int* mult_matrix;
	mult_matrix = new int[N * N];
	
	for (int i = 0; i < N; i++) 
	{
		for (int j = thread; j < N; j += thread_size) 
		{
			mult_matrix[i * N + j] = 0;
			
			for (int k = 0; k < N; k++) 
			{
				mult_matrix[i * N + j] += matrix1[i * N + k] * matrix2[k * N + j];
			}
		}
	}

	return mult_matrix;
}

