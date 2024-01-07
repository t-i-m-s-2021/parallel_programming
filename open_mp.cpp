#include <iostream>
#include <omp.h>

#define N 2000

using namespace std;

int** create_matrix() {
	int** matrix = new int* [N];
	for (int i = 0; i < N; i++)
		matrix[i] = new int[N];

	return matrix;
}

int** random_matrix(int** matrix) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			matrix[i][j] = rand() % N;

	return matrix;
}

int main() {
	int **matrix1, **matrix2, **mult_matrix;
	double start, end;
	int max_threads = omp_get_max_threads();
	matrix1 = random_matrix(create_matrix());
	matrix2 = random_matrix(create_matrix());
	mult_matrix = create_matrix();
	int i, j, k;
		for (int i = 1; i <= max_threads; i++) {
			start = omp_get_wtime();
			omp_set_num_threads(i);
			#pragma omp parallel for shared(matrix1, matrix2, mult_matrix) private(i, j, k)
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					mult_matrix[i][j] = 0;
					for (int k = 0; k < N; k++) {
						mult_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
					}
				}
			}
			end = omp_get_wtime();

			cout << "Count thread = " << i << ", execution time = " << end - start << "s\n";
		}
		return 0;
}